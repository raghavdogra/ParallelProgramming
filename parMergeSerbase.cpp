using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <cilk/cilk.h>
#include <iostream>
long COUNT = 8;
long * A;
long * T;
struct timeval start,end;

long base = 5, insertbase = 1;
void swap(long *a, long *b) {
     long t = *a;
    *a = *b;
    *b = t;
}

long binarySearch( const long* a, long value, long start, long end )
{
    long low  = start;
    long high; 
    if (start > end + 1) 
       {
	 high = start;
       }
    else {
 	 high = end + 1;
	}	
    while( low < high )
    {
        long mid = ( low + high ) / 2;
        if ( value <= a[ mid ] )
		{
			 high = mid;
		}
        else     
                {
			low  = mid + 1;
		}
    }
    return high;
}


void smerge(long p1, long r1, long p2, long r2, long p3, long* T, long* A)
{
    long i, j, k;
    long n1 = r1 - p1 + 1;
    long n2 = r2 - p2 + 1;

    long *L = NULL, *R = NULL;

    L = new long[n1];
    R = new long[n2];

    for (i = 0; i < n1; i++)
        L[i] = T[p1 + i];
    for (j = 0; j < n2; j++)
        R[j] = T[p2 + j];

    i = 0;
    j = 0;
    k = p3;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            A[k] = L[i];
            i++;
        }
        else
        {
            A[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1)
    {
        A[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        A[k] = R[j];
        j++;
        k++;
    }
    delete [] L;
    delete [] R;
}


void pmerge( long* T, long p1, long r1, long p2, long r2, long* A, long p3 )
{
    long n1 = r1 - p1 + 1;
    long n2 = r2 - p2 + 1;
    if ( n1 < n2 ) {
        swap(&p1,  &p2 );
        swap(&r1,  &r2 );
        swap( &n1, &n2 );
    }
    if ( n1 + n2 <= base) {
          smerge( p1, r1, p2, r2, p3 ,T, A);
          return ;
        }

    long q1 = ( p1 + r1 ) / 2;
    long q2 = binarySearch( T, T[ q1 ], p2, r2 );
    long q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
    A[q3] = T[q1];

    cilk_spawn pmerge( T, p1, q1 - 1, p2, q2 - 1, A, p3);
    pmerge( T, q1 + 1, r1, q2, r2, A, q3 + 1 );
    cilk_sync;

}

void insertionSort(long * A, long len) {
	for (long i = 0; i < len; i++) {
	long j = i;
	while (j > 0 && A[j-1] > A[j]) {
		long temp = A[j-1];
		A[j-1] = A[j];
		A[j] = temp;
		j--;
		        }
		    }
		}

void pmergesort( long* A, long p, long r, long* T, bool switc)
{
    if ( r == p) {
        if (switc)  
	T[p] = A[p];
        return;
    }

    if (( r - p ) <= insertbase && !switc ) {
        insertionSort( A + p, r - p + 1 );
        return;
    }
    long q = (( r + p ) / 2 );
    cilk_spawn pmergesort( A, p, q, T, !switc );
    pmergesort( A, q + 1, r, T, !switc );
    cilk_sync;
    if (switc)
	{
    		pmerge( A, p, q, q + 1, r, T, p );
	}
        else     
	{ 
		pmerge( T, p, q, q + 1, r, A, p );
	}
}


  int main (int argc, const char * argv[]) {

  long n = atoi(argv[1]);
  COUNT = (1 << 26)*n;
  insertbase = 1 << atoi(argv[2]);
  base = 1 << atoi(argv[3]);
  
  A = new long[COUNT];
  T = new long[COUNT];
  long i = 0;
  srand(time(NULL));
  for (long i= 0; i < COUNT; i++)
 	{ 
		A[i] = rand() %100;
	}
/*  for (i= 0; i < COUNT; i++)
 	{ 
		printf("%d ", A[i]);
	}
*/
  gettimeofday(&start,NULL); //Start timing the computation 
  pmergesort(A, 0, COUNT-1, T, true);
  gettimeofday(&end,NULL); //Stop timing the computation
  double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
  cout << "Parallel MergeSort with parallel merge and base case serial merge implemented in " << myTime << " seconds.\n";

  /*printf("\n");
  for (i= 0; i < COUNT; i++)
 	{ 
		printf("%d ", T[i]);
	}*/
}
