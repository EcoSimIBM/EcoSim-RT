#!/bin/bash
#SBATCH --cpus-per-task=1   # maximum CPU cores per GPU request: 6 on Cedar, 16 on Graham.
#SBATCH --mem=8192M        # memory per node
#SBATCH --time=7-00:00      # time (DD-HH:MM)
#SBATCH --output=EcoSimOut.txt
#SBATCH --account=rrg-rgras-ad

./EcoSim
