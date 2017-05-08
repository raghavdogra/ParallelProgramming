#!/bin/bash
#SBATCH -J ParallelMerge        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 16           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 1.5 hours
#export CILK_NWORKERS=16
rm -f ssort_varbc
for((processor=6; processor <= 11; processor++))
do
                echo " $processor"  >> ssort_varbc
    ./rssp 12 $processor  >> ssort_varbc
done

