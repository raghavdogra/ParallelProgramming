#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include "parallel_merge.h"

template <typename T>
void swap(T *a, T *b) {
	T temp = *a;
	*a = *b;
	*b = temp;
}

void printarr(int *arr, int len) {
	int i;
	for(i=0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");
}

long  binary_search( int value, const int* a, long left, long right )
{
	long low  = left;
	long high =  left > right + 1 ? left : right + 1;
	while( low < high )
	{
		long mid = ( low + high ) / 2;
		if ( value <= a[ mid ] ) high = mid;
		else     low  = mid + 1;
	}
	return high;
}

void merge(long p1, long r1, long p2, long r2, long p3, int* t, int* a)
{
    long i, j, k;
    long n1 = r1 - p1 + 1;
    long n2 = r2 - p2 + 1;

    int *L = NULL, *R = NULL;

    L = new int[n1];
    R = new int[n2];
 
    for (i = 0; i < n1; i++)
        L[i] = t[p1 + i];
    for (j = 0; j < n2; j++)
        R[j] = t[p2 + j];
 
    i = 0;
    j = 0;
    k = p3;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            a[k] = L[i];
            i++;
        }
        else
        {
            a[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        a[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        a[k] = R[j];
        j++;
        k++;
    }
    delete [] L;
    delete [] R;
}


void pmerge( int* t, long p1, long r1, long p2, long r2, int* a, long p3 )
{
	long length1 = r1 - p1 + 1;
	long length2 = r2 - p2 + 1;
	if ( length1 + length2 <= BASE_PMERGE) {
		merge( p1, r1, p2, r2, p3 ,t, a);
		return ;
	}
	if ( length1 < length2 ) {
		swap<long>(&p1,  &p2 );
		swap<long>(&r1,  &r2 );
		swap<long>( &length1, &length2 );
	}
//	if ( length1 == 0 ) return;
	long q1 = ( p1 + r1 ) / 2;
	long q2 = binary_search( t[ q1 ], t, p2, r2 );
	long q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
	a[ q3 ] = t[ q1 ];

	cilk_spawn pmerge( t, p1,     q1 - 1, p2, q2 - 1, a, p3     );
		   pmerge( t, q1 + 1, r1,     q2, r2,     a, q3 + 1 );
	cilk_sync;
}

void insertSort(int *A, int len) {
	for (int i = 0; i < len; i++) {
		int j = i;
		while (j > 0 && A[j-1] > A[j]) {
			swap<int>(&A[j], &A[j-1]);
			j--;
		}
	}
}

void pmergesort(int* from, long p, long r, int* dst, bool fromToDst)
{
	if (r == p) {
		if (fromToDst)  dst[p] = from[p];
		return;
	}

	if ((r - p + 1) <= BASE_LEN  && !fromToDst ) {
		insertSort( from + p, r - p + 1 );
		return;
	}

	long m = ((r + p) / 2);
	cilk_spawn pmergesort(from, p,     m, dst, !fromToDst);      
		   pmergesort(from, m + 1, r, dst, !fromToDst);      
	sync;

	if (fromToDst)
		pmerge( from, p, m, m + 1, r, dst, p);
	else    
		pmerge(dst, p, m, m + 1, r, from, p);
}


int main (int c, char *args[]) {
	int n = atoi(args[1]);
	struct timeval start,end;
	ARR_SIZE = 1 << n;
	BASE_LEN = 1 << atoi(args[2]);
	BASE_PMERGE = 1 << atoi(args[3]);

	cout << "Run with arr_size: " << ARR_SIZE << "\n";
	cout << "Base Length: " << BASE_LEN << "\n" ;
	cout << "Base P merge: " << BASE_PMERGE << "\n";
	init_arr();
	gettimeofday(&start,NULL); //Start timing the computation
	pmergesort(A, 0, ARR_SIZE-1, T, true);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Parmsort: " << myTime << " seconds.\n";
//	printarr(A,ARR_SIZE);
//	printarr(T,ARR_SIZE);
	destroy_arr();
}
