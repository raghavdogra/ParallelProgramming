#!/bin/bash
#SBATCH -J ParallelMerge        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 16           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 1.5 hours
#export CILK_NWORKERS=4
#./pmsort_pmerge 8 3 8 > pm8_38
#export CILK_NWORKERS=5
#./pmsort_pmerge 10 3 8 > pm10_38
#export CILK_NWORKERS=6
#./pmsort_pmerge 12 3 8 > pm12_38
#input=30
#max=30
#min=13
rm -f pmsort_smerge_30size
for((processor=1; processor <= 16; processor++))
do
                echo " $processor"  >> pmsort_smerge_30size
                export CILK_NWORKERS=$processor
    ./pmsort_smerge $processor 3 15 >> pmsort_smerge_30size
done
