To run, please modify the run.sbatch file line 21.
 The line looks like:

 { time ./crawler "Elvis" 2; } 2>&1

 edit "Elvis" with the starting node, and "2" that's within the bracket with the depth.

 After editing, run "sbatch run.sbatch"

 The output file from the job will contain the execution time at the bottom of the output file.
