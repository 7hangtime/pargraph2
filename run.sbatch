#!/bin/bash
#SBATCH --job-name=graph_crawler  
#SBATCH --output=graph_crawler_%j.out       
#SBATCH --ntasks=1                           
#SBATCH --cpus-per-task=6                    #  THIS NEEDS TO MATCH MAX THREADS
#SBATCH --time=00:10:00                      
#SBATCH --partition=Centaurus               

make

# Print job info at the top of the .out file
echo "======================================"
echo " SLURM job information"
echo " Job ID: $SLURM_JOB_ID"
echo " Job Name: $SLURM_JOB_NAME"
echo " Node: $SLURM_NODELIST"
echo " CPUs: $SLURM_CPUS_PER_TASK"
echo "======================================"
echo ""

# Run with start node "Yola", depth 3, and 4 threads
{ time ./crawler "Yola" 5 6; } 2>&1

rm crawler
