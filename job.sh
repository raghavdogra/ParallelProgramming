#!/bin/bash
#SBATCH -J Test_multiplication        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 1           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 00:30:00     # Run time (hh:mm:ss) - 1.5 hours
export CILK_NWORKERS=1
./a > a1_output
