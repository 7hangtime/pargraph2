To run, please modify the run.sbatch file line 22
 The line looks like:

 { time ./crawler "Yola" 5 6; } 2>&1

 edit "Yola" with the starting node. The following numbers are the depth, and number of threads respectivlely.
 You will also need to edit line 5 with the number of cpus to match the number of threads. This information is also commented
 in the sbatch file.

 After editing, run "sbatch run.sbatch"

 The output file from the job will contain the execution time at the bottom of the output file.
