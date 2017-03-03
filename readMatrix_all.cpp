#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cilk/cilk.h>


using namespace std;
#define COUNT 4096


int main () {
  string line;
  char s1, s2;
  int i, j,k, value;
  int** X = new int*[COUNT];
  int** Y = new int*[COUNT];
  int** Z = new int*[COUNT];
  for(i = 0; i < COUNT; i++)
  {
	X[i] = new int[COUNT];
	Y[i] = new int[COUNT];
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
	}
	i++;      
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
	}
	i++;      
  }
    matrix2.close();
}

  else cout << "Unable to open file";
} 

cout<< "Reading Finished\n";
s2 = 'y';
if (s2 =='y')
{


clock_t start = clock();

 for(i=0;i<COUNT;i++)
    {
        for(j=0; j<COUNT; j++)
        {
                for(k=0; k<COUNT; k++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }

  printf ( "i-j-k : %f\n", ( (double)clock() - start ) / CLOCKS_PER_SEC );

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;



clock_t start1 = clock();

 for(i=0;i<COUNT;i++)
    {
        for(k=0; k<COUNT; k++)
        {
                for(j=0; j<COUNT; j++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }

  printf ( "i-k-j : %f\n", ( (double)clock() - start1 ) / CLOCKS_PER_SEC );

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;


clock_t start2 = clock();

 for(j=0;j<COUNT;j++)
    {
	for(i=0; i<COUNT; i++)
        {
		for(k=0; k<COUNT; k++)
		{
			Z[i][j] =+ X[i][k] * Y[k][j];
		}	  
	}
    }	
  
  printf ( "j-i-k : %f\n", ( (double)clock() - start2 ) / CLOCKS_PER_SEC );
/*

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;


clock_t start3 = clock();

 for(j=0;j<COUNT;j++)
    {
        for(k=0; k<COUNT; k++)
        {
                for(i=0; i<COUNT; i++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }

  printf ( "j-k-i : %f\n", ( (double)clock() - start3 ) / CLOCKS_PER_SEC );

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;


clock_t start4 = clock();

 for(k=0;k<COUNT;k++)
    {
        for(i=0; i<COUNT; i++)
        {
                for(j=0; j<COUNT; j++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }

  printf ( "k-i-j : %f\n", ( (double)clock() - start4 ) / CLOCKS_PER_SEC );

for(i=0;i<COUNT;i++)
  for(j=0;j<COUNT;j++)
     Z[i][j] = 0;


clock_t start5 = clock();

 for(k=0;k<COUNT;k++)
    {
        for(j=0; j<COUNT; j++)
        {
                for(i=0; i<COUNT; i++)
                {
                        Z[i][j] =+ X[i][k] * Y[k][j];
                }
        }
    }

  printf ( "k-j-i : %f\n", ( (double)clock() - start5 ) / CLOCKS_PER_SEC );

*/
}
  delete [] X;
  delete [] Y;
  delete [] Z;	
  return 0;
}
