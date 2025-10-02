CXX = g++
CXXFLAGS = -std=c++17 -Wall -I.    # add -I. so local rapidjson headers are found
LIBS = -lcurl

all: crawler

crawler: graphcrawler.cpp
	$(CXX) $(CXXFLAGS) graphcrawler.cpp -o crawler $(LIBS)


