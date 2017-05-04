using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <cilk/cilk.h>
#include <iostream>
#include "parallel_merge.h"
struct timeval start,end;


void R_SS(int *A,int l ,int h,int n);
void P_SS(int *A, int ll,int lh,int rl,int rh,int n);

void R_SS(int *A,int l ,int h,int n)
{
	int temp, m, i,j,min;
	if((h-l + 1) <= 2)
	{
		for(i = l; i <=h-1; i++)
		{
			min = i;
			for(j = i+1; j <= h; j++)
			{
				if(A[j] < A[min])
				{
					min = j;
				}
			}
			if (min != i)
			{
					temp = A[i];
					A[i] = A[min];
					A[min] = temp;
			}
		}
	}
	else
	{
		m = (l+h) / 2;
		P_SS(A,l,m,m+1,h,n);
		cilk_spawn R_SS(A, l, m, n);
		R_SS(A, m+1, h, n);
	}
}
	
void P_SS(int *A, int ll,int lh,int rl,int rh,int n)
{
int temp, lm,rm,i,j,min ;	
	if((lh - ll + 1) <= 2)
	{
		for(i = ll; i <= lh; i++)
		{
			min = i;
			for(j = rl; j <= rh; j++)
			{
				if(A[j] < A[min])
				{
					min = j;
				}
			}
			if (min != i)
			{
					temp = A[i];
					A[i] = A[min];
					A[min] = temp;
			}

		}
	}
	else
	{
		lm = (ll + lh) / 2;
		rm = (rl + rh) / 2;
		cilk_spawn P_SS(A,ll,lm,rl,rm,n);
		P_SS(A,lm+1,lh,rm+1,rh,n);
		cilk_spawn P_SS(A,ll,lm,rm+1,rh,n);
		P_SS(A,lm+1,lh,rl,rm,n);
	}
}

int main(int c, char *args[])
{
	int n = atoi(args[1]);
	int i;
	srand(time(NULL));
	
	ARR_SIZE = (1 << n);
	cout << "Run with arr_size: " << ARR_SIZE << "\n";
	init_arr();
	//for(i = 0; i< 10; i++)
	//	{
	//	int r = rand();
	//	A[i] = 10-i;
	//}
    gettimeofday(&start,NULL);
    R_SS(A,0,ARR_SIZE-1,ARR_SIZE);
    gettimeofday(&end,NULL);
    double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
    cout << "Parallel selection sort done in " << myTime << " seconds.\n";
    destroy_arr();
	//      for(i = 0; i< 10; i++)
     //     {
    //		  printf("%d\t",A[i]);
      //    }
	
}

