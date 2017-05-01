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


void R_BS(int *A,int l ,int h,int n);
void P_BS(int *A, int ll,int lh,int rl,int rh,int n);

void R_BS(int *A,int l ,int h,int n)
{
	int temp, m, i,j;
	if((h-l + 1) <= 2)
	{
		for(i = l; i <=h; i++)
		{
			for(j = l; j <= l + h - i - 1; j++)
			{
				if(A[j] > A[j+1])
				{
					temp = A[j];
					A[j] = A[j+1];
					A[j+1] = temp;
				}
			}
		}
	}
	else
	{
		m = (l+h) / 2;
		P_BS(A,l,m,m+1,h,n);
		cilk_spawn R_BS(A, l, m, n);
		R_BS(A, m+1, h, n);
	}
}
	
void P_BS(int *A, int ll,int lh,int rl,int rh,int n)
{
int temp, lm,rm,i,j ;	
	if((lh - ll + 1) <= 2)
	{
		for(i = rl; i <= rh; i++)
		{
			for(j = ll; j <lh; j++)
			{
				if(A[j] > A[j+1])
				{
					temp = A[j];
					A[j] = A[j+1];
					A[j+1] = temp;
				}
			}
		if(A[lh] > A[i])
		{
			temp = A[lh];
			A[lh] = A[i];
			A[i] = temp;
		}
		}
	}
	else
	{
		lm = (ll + lh) / 2;
		rm = (rl + rh) / 2;
		cilk_spawn P_BS(A,ll,lm,rl,rm,n);
		P_BS(A,lm+1,lh,rm+1,rh,n);
		cilk_spawn P_BS(A,ll,lm,rm+1,rh,n);
		P_BS(A,lm+1,lh,rl,rm,n);
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
	gettimeofday(&start,NULL);
	R_BS(A,0,ARR_SIZE-1,ARR_SIZE);
	gettimeofday(&end,NULL); 
	 double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
    	cout << "Parallel Bubble Sort Done in " << myTime << " seconds.\n";
	destroy_arr();
}

