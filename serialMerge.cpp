#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <cilk/cilk.h>

#define COUNT  10 

void mergeSort(int * A, int p, int r);
void merge(int * A, int p, int q, int r);
void insertionSort(int * A, int p, int r);
int main() {
			
	int *A = new int[COUNT];
	srand(time(NULL));	
	for (int i = COUNT - 1; i >= 0; i--) {
		A[i] = rand() %100;	
	}
	for (int i = 0; i < COUNT; i++) {	
		printf("%d ", A[i]);
	}
	mergeSort(A,0, COUNT - 1);		
	printf("\n");
	for (int i = 0; i < COUNT; i++) {
		printf("%d ", A[i]);
	}
	delete [] A;

}
		
void insertionSort(int * A, int len) {
	for (int i = 0; i < len; i++) {
	int j = i;
	while (j > 0 && A[j-1] > A[j]) {
		int temp = A[j-1];
		A[j-1] = A[j];
		A[j] = temp;
		j--;
		        }
		    }
		}
void mergeSort(int * A, int p, int r)
{
	 if ( r - p + 1 <= 16)
	 {
		insertionSort(A + p,r - p + 1 );
	 }
	else {
		if(p < r) {
			int q = (p + r) / 2;
			cilk_spawn mergeSort(A, p, q);
			mergeSort(A, q + 1,r);
			cilk_sync;
			merge(A, p, q, r);	   
			}
		}
}
		 
void merge(int * A, int p, int q, int r)
	{
		int n = r-p+1;
		int * T = new int[n];
		for(int ind = 0; ind < n; ind++)
			 {
				 T[ind] = A[p + ind];
			 }
		int i = 0;
		int j = ((q - p)) + 1;
		int k = p;
		while (i <= (q - p) && j < n) 
			 { 
				   if(T[i] <= T[j])
				  	{
				  		A[k] = T[i];
				  		i++;
				  		k++;
				  	}
				  else 
					{
				 		A[k] = T[j];
				 		j++;
				 		k++;
			  		}
			 }
			 while( j < n)
			 {
				  A[k] = T[j];
					 k++;
					 j++;
				
			 }
			 while( i <= (q - p))
			 {
				  A[k] = T[i];
					 i++;
					 k++;
			 }

		delete [] T;				
		 }
