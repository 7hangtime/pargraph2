#!/bin/bash
#SBATCH --job-name=graph_crawler  
#SBATCH --output=graph_crawler_%j.out       
#SBATCH --ntasks=1                           
#SBATCH --cpus-per-task=1                    
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

{ time ./crawler "Yola" 3; } 2>&1
rm crawler
