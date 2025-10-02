// David Thrower
// Grpah Crawler using BFS
// 801232129

/*
    I wanted to use my original graphcrawler.cpp, so I took the BFS impl from the provided
    Sequential version that's in the assignment tar.gz. From there, I just modified my main func
    to use it, and then from there I added the paralalelism using threads and mutexes.
    Since it says that we could have just used the entire code from the sequential version,
    I figured this wouldn't be a form of "plagiarism".
*/

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <curl/curl.h>
#include "rapidjson/document.h"
#include <unordered_set>

using namespace std;
using namespace rapidjson;

std::mutex mut;

// This callback is needed because libcurl will just print the data to console by default

size_t callback(char *ptr, size_t size, size_t num, void *userdata)
{
    std::string *buffer = (std::string *)userdata;
    buffer->append(ptr, size * num);
    return size * num;
}

// Nothing changed here from the origin graphcrawler assignment
vector<string> getNeighbors(const string &node)
{
    vector<string> neighbors;
    CURL *curl = curl_easy_init();

    string read;
    string url = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

    char *encoded = curl_easy_escape(curl, node.c_str(), node.length()); // Makes the node string passed from cmdline the right format -> Tom Hanks goes to Tom%20Hanks.
    if (encoded)
    {
        url += encoded;
        curl_free(encoded); // no mem leaks plz
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());        // Set the URL
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback); // runs callback
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read);        // The data from callback goes into read
    curl_easy_perform(curl);                                 // the actual request pull

    Document d;
    d.Parse(read.c_str());

    for (auto &v : d["neighbors"].GetArray())
    {
        neighbors.push_back(v.GetString());
    }

    curl_easy_cleanup(curl);
    return neighbors;
}

// This is a modified version of the BFS from the sequential version provided to us
std::vector<std::vector<std::string>> bfs_parallel(const std::string &start, int depth, int maxThreads)
{
    std::vector<std::vector<std::string>> levels;
    std::unordered_set<std::string> visited;

    levels.push_back({start});
    visited.insert(start);

    for (int d = 0; d < depth; d++)
    {
        levels.push_back({});

        int numNodes = levels[d].size();
        int numThreads = std::min(maxThreads, numNodes);
        int chunk = (numNodes + numThreads - 1) / numThreads;

        std::vector<std::thread> threads;

        for (int t = 0; t < numThreads; t++)
        {
            int start = t * chunk;
            int end = std::min(start + chunk, numNodes);

            threads.push_back(std::thread([&, start, end]()
                                          {
                std::vector<std::string> localVector; // each thread has it's own vector so I'm not dumping everything in levels for each node

                for (int i = start; i < end; i++) {
                    std::string s = levels[d][i];
                    for (const std::string& neighbor : getNeighbors(s)) {
                        std::lock_guard<std::mutex> lock(mut);
                        if (!visited.count(neighbor)) {
                            visited.insert(neighbor);
                            localVector.push_back(neighbor);
                        }
                    }
                }

                std::lock_guard<std::mutex> lock(mut);
                levels[d+1].insert(levels[d+1].end(), localVector.begin(), localVector.end()); }));
        }

        for (int j = 0; j < threads.size(); j++)
        {
            threads[j].join();
        }
    }

    return levels;
}

int main(int argc, char *argv[])
{

    // TA recommended I add this in feedback for last assignment
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <start_node> <depth> <max_threads>\n";
        return 1;
    }

    string startNode = argv[1];
    int depth = stoi(argv[2]);
    int maxThreads = stoi(argv[3]);


    // Modified this to use the bfs from the sequential version provided to us + Show the levels
    // Asked in the assignment PDF
    std::vector<std::vector<std::string>> levels = bfs_parallel(startNode, depth, maxThreads);

    cout << "Nodes reachable within depth " << depth << ":\n";
    for (int d = 0; d < levels.size(); d++)
    {
        cout << "Level " << d << " (" << levels[d].size() << " nodes):\n";
        for (const std::string &node : levels[d])
        {
            cout << "- " << node << "\n";
        }
    }
    return 0;
}