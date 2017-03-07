#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cilk/cilk.h>


using namespace std;
#define COUNT 8192

void matMultiply(int Zx0, int Zxn, int Zy0,int Zyn, int Xx0, int Xxn, int Xy0, int Xyn, int Yx0, int Yxn, int Yy0,int  Yyn, int n); 
  int** X = new int*[COUNT];
  int** Y = new int*[COUNT];
  int** Z = new int*[COUNT];

int main () {
  string line;
  char s1, s2;
  int i, j,k, value;
  struct timeval start,end;
for(i = 0; i < COUNT; i++)
  {
        X[i] = new int[COUNT];
  }

  for(i = 0; i < COUNT; i++)
  {
        Y[i] = new int[COUNT];
  }

  for(i = 0; i < COUNT; i++)
  {
        Z[i] = new int[COUNT]();
  }

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;
  
  ifstream matrix1 ("matrix1.txt");
  if (matrix1.is_open())
  {
    i = 0;
    j = 0;
    while ( getline (matrix1,line) )
    {	
	std::istringstream ss(line);	
	j=0;	
	while (ss >> value)
	{
		X[i][j] = value;;
        	if (ss.peek() == ',' || ss.peek() == ' ')
            	ss.ignore();
		j++;
		if (j==COUNT)
			break;
	}
	i++;
	if (i==COUNT)
		break;      
  }
    matrix1.close();
}

  else cout << "Unable to open file";
  
cout << "read Matrix \n";

s1 = 'y';


if(s1=='y')
{  
  ifstream matrix2 ("matrix2.txt");
  if (matrix2.is_open())
  {
    i = 0;
    j = 0;
    while ( getline (matrix2,line) )
    {	
	std::istringstream ss(line);	
	j=0;	
	while (ss >> value)
	{
		Y[i][j] = value;;
        	if (ss.peek() == ',' || ss.peek() == ' ')
            	ss.ignore();
		j++;
		if (j==COUNT)
			break;
	}
	i++;      
	if (i==COUNT)
		break;      
  }
    matrix2.close();
}

  else cout << "Unable to open file";
} 

cout<< "Reading Finished\n";
s2 = 'y';
if (s2 =='y')
{

gettimeofday(&start,NULL); //Start timing the computation

matMultiply(0,COUNT-1,0,COUNT-1,0,COUNT-1,0,COUNT-1,0,COUNT-1,0,COUNT-1, COUNT);

gettimeofday(&end,NULL); //Stop timing the computation
double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

cout << "Count is " << COUNT<<"\n";

cout << "k-i-j i-j_cilk  implemented in " << myTime << " seconds.\n";


/* for(int k=0;k<COUNT;k++)
    {
        cilk_for(int i=0; i<COUNT; i++)
        {
                cilk_for(int j=0; j<COUNT; j++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }
gettimeofday(&end,NULL); //Stop timing the computation
double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

cout << "Count is " << COUNT<<"\n";

cout << "k-i-j i-j_cilk  implemented in " << myTime << " seconds.\n";

gettimeofday(&start,NULL); //Start timing the computation

 for(int k=0;k<COUNT;k++)
    {
        for (int i=0; i<COUNT; i++)
        {
                cilk_for(int j=0; j<COUNT; j++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }
gettimeofday(&end,NULL); //Stop timing the computation
myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

cout << "k-i-j j_cilk implemented in " << myTime << " seconds.\n";

gettimeofday(&start,NULL); //Start timing the computation

 for(int k=0;k<COUNT;k++)
    {
        cilk_for(int i=0; i<COUNT; i++)
        {
                for (int j=0; j<COUNT; j++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }
gettimeofday(&end,NULL); //Stop timing the computation
myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);

cout << "k-i-j i_cilk implemented in " << myTime << " seconds.\n";
*/
}
  delete [] X;
  delete [] Y;
  delete [] Z;	
  return 0;
}

void matMultiply(int Zx0, int Zxn, int Zy0,int Zyn, int Xx0, int Xxn, int Xy0, int Xyn, int Yx0, int Yxn, int Yy0,int  Yyn, int n) {
		if(n == 2) {
			for(int k =0;k < n; k++ ) {
				for(int i = Zx0; i <= Zxn; i++) {
					for(int j = Zy0; j<= Zyn; j++) {
						Z[i][j] = Z[i][j] + X[i][Xy0 + k] * Y[Yx0 + k][j]; 
					}
				}
			}
		}
		else {
			
			// {0,1,0,1} = Zx0,Zx0 +( n/2 -1),Zy0,Zy0 +( n/2 -1)
			// {2,3,0,1} = Zx0 + n/2,Zx0 + (n-1),Zy0,Zy0 +( n/2 -1)
			// {0,1,2,3} = Zx0,Zx0 +( n/2 -1),Zy0 + n/2,Zy0 + (n-1)
			// {2,3,2,3} = Zx0 + n/2,Zx0 + (n-1),Zy0 + n/2,Zy0 + (n-1)
	cilk_spawn matMultiply(Zx0,Zx0 +( n/2 -1),Zy0,Zy0 +( n/2 -1),Xx0,Xx0 +( n/2 -1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0,Yy0 +( n/2 -1),n/2);
	cilk_spawn matMultiply(Zx0,Zx0 +( n/2 -1),Zy0 + n/2,Zy0 + (n-1),Xx0,Xx0 +( n/2 -1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0 + n/2,Yy0 + (n-1),n/2);
	cilk_spawn matMultiply(Zx0 + n/2,Zx0 + (n-1),Zy0,Zy0 +( n/2 -1),Xx0 + n/2,Xx0 + (n-1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0,Yy0 +( n/2 -1),n/2);
         matMultiply(Zx0 + n/2,Zx0 + (n-1),Zy0 + n/2,Zy0 + (n-1),Xx0 + n/2,Xx0 + (n-1),Xy0,Xy0 + (n-1),Yx0,Yx0 +( n/2 -1),Yy0 + n/2,Yy0 + (n-1),n/2);
		cilk_sync;	
	cilk_spawn matMultiply(Zx0,Zx0 +( n/2 -1),Zy0,Zy0 +( n/2 -1),Xx0,Xx0 +( n/2 -1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0,Yy0 +( n/2 -1),n/2);
	cilk_spawn matMultiply(Zx0,Zx0 +( n/2 -1),Zy0 + n/2,Zy0 + (n-1),Xx0,Xx0 +( n/2 -1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0 + n/2,Yy0 + (n-1),n/2);
	cilk_spawn matMultiply(Zx0 + n/2,Zx0 + (n-1),Zy0,Zy0 +( n/2 -1),Xx0 + n/2,Xx0 + (n-1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0,Yy0 +( n/2 -1),n/2);
        matMultiply(Zx0 + n/2,Zx0 + (n-1),Zy0 + n/2,Zy0 + (n-1),Xx0 + n/2,Xx0 + (n-1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0 + n/2,Yy0 + (n-1),n/2);	
	cilk_sync;	

		     }
}
