#! /usr/bin/sh
input=30
max=30
min=13
rm -f pmsort_pmerge_30size
for((i=$min; i <= $max; i++))
do
	for ((j=6; j <= $max; j++))
	do
		echo "$i, $j"  >> pmsort_pmerge_30size
		./pmsort_pmerge $input $i $j >> pmsort_pmerge_30size
	done
done
