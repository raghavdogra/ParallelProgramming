#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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
		R_SS(A, l, m, n);
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
		P_SS(A,ll,lm,rl,rm,n);
		P_SS(A,lm+1,lh,rm+1,rh,n);
		P_SS(A,ll,lm,rm+1,rh,n);
		P_SS(A,lm+1,lh,rl,rm,n);
	}
}

void main()
{
	int i, A[10];
	srand(time(NULL));
	
	for(i = 0; i< 10; i++)
	{
	//	int r = rand();
		A[i] = 10-i;
	}
	
	for(i = 0; i< 10; i++)
	{
		printf("%d\t",A[i]);
	}
	R_SS(A,0,9,10);
	printf("\n\n");
	for(i = 0; i< 10; i++)
	{
		printf("%d\t",A[i]);
	}
	printf("\n\n");
}

