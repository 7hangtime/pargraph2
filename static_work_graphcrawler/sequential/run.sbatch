#!/bin/bash
#SBATCH --job-name=level_client  
#SBATCH --output=level_client_%j.out       
#SBATCH --ntasks=1                           
#SBATCH --cpus-per-task=1                    
#SBATCH --time=00:10:00                      
#SBATCH --partition=Centaurus               

make level_client

echo "======================================"
echo " SLURM job information"
echo " Job ID: $SLURM_JOB_ID"
echo " Job Name: $SLURM_JOB_NAME"
echo " Node: $SLURM_NODELIST"
echo " CPUs: $SLURM_CPUS_PER_TASK"
echo "======================================"
echo ""


# Change the number after Yola to for depth
{ time ./level_client "Yola" 3; } 2>&1

# Clean up
rm level_client

