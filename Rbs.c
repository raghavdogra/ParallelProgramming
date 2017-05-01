#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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
		R_BS(A, l, m, n);
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
		P_BS(A,ll,lm,rl,rm,n);
		P_BS(A,lm+1,lh,rm+1,rh,n);
		P_BS(A,ll,lm,rm+1,rh,n);
		P_BS(A,lm+1,lh,rl,rm,n);
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
	R_BS(A,0,9,10);
	printf("\n\n");
	for(i = 0; i< 10; i++)
	{
		printf("%d\t",A[i]);
	}
	printf("\n\n");
}

