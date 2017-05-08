#!/bin/bash
#SBATCH -J ParallelMerge        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 16           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 1.5 hours
#export CILK_NWORKERS=16
rm -f bsort_varpro
for i in {2..15}
do
        export CILK_NWORKERS=$i
                echo " $i"  >> bsort_varpro
    ./RbsP 20 5 >> bsort_varpro
done
