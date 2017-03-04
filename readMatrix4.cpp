#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cilk/cilk.h>


using namespace std;
#define COUNT 128


int main () {
  string line;
  char s1, s2;
  int i, j,k, value;
  struct timeval start,end;
  int** X = new int*[COUNT];
  int** Y = new int*[COUNT];
  int** Z = new int*[COUNT];
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

 for(int k=0;k<COUNT;k++)
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



/*
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
