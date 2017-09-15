#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>
#include <cilk/cilk.h>
#include <limits.h>
#include <stdlib.h>
using namespace std;
//#define COUNT 8192


void matMultiply(int Zx0, int Zxn, int Zy0,int Zyn, int Xx0, int Xxn, int Xy0, int Xyn, int Yx0, int Yxn, int Yy0,int  Yyn, int n);

void Apar(int Xi,int Xj, int Xn);

void Bpar(int Xi,int Xj, int Xn, int Ui, int Uj, int Un, int Vi,int Vj, int Vn);

void Cpar(int Xi,int Xj, int Xn, int Ui, int Uj, int Un, int Vi,int Vj, int Vn);

void SerPar(int Xi,int Xj, int Xn);

void printZ(void);

void Aloopser(int Xi, int Xj, int Xn);

void Bloopser(int Xi, int Xj, int Xn, int, int, int, int);

void Cloopser(int Xi, int Xj, int Xn, int, int, int, int);

int mymin(int x, int y);

int** X;
int** Y;
int** Z;
//Z[8][8];
int m = 8;
int COUNT;
int main(int argc, const char * argv[]) {
    // insert code here...
    string line;
    char s1, s2;
    int i, j,k, value;
    struct timeval start,end;
 //   COUNT = atoi(argv[1]);
    COUNT = 16;
    X = new int*[COUNT];
    Y = new int*[COUNT];
    Z = new int*[COUNT];

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
            Z[i][j] = INT_MAX/15;
    
    for(i=0;i<COUNT-1;i++)
        Z[i][i+1]=1;  
    
    
/*    ifstream matrix1 ("matrix1.txt");
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
    
  */  
    
//    std::cout << "Hello, World!\n";
  //  printf("Min of 4 5 = %d \n", min(4,5));
   // printZ();
   gettimeofday(&start,NULL); //Start timing the computation 
    Apar(0,0,COUNT);
    gettimeofday(&end,NULL); //Stop timing the computation
    double myTime = (end.tv_sec+(double)end.tv_usec/1000000) - (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Count is " << COUNT<<"\n";
	cout << "  implemented in " << myTime << " seconds.\n";

  //  Aloopser(0,0,COUNT);
   // SerPar(0,0,COUNT);
    printZ();
     delete [] X;
delete [] Y;
delete [] Z;
    return 0;
}


void Apar(int Xi,int Xj, int Xn)
{
    if (Xn > m)
    {
        cilk_spawn Apar(Xi,Xj,Xn/2);   //Apar( X11 )
        Apar(Xi+(Xn/2),Xj+(Xn/2),Xn/2);     //Apar( X22 )
	cilk_sync;
        Bpar(Xi,Xj+(Xn/2),Xn/2,Xi,Xj,Xn/2,Xi+(Xn/2),Xj+(Xn/2),Xn/2); //Bpar( X12, X11, X22 )
    }
    else
    {
        Aloopser(Xi,Xj,Xn);
    }
}


void Bpar(int Xi,int Xj, int Xn, int Ui, int Uj, int Un, int Vi,int Vj, int Vn)
{
    if (Xn > 1)
    {
        Bpar(Xi+(Xn/2),Xj,Xn/2, Ui+(Un/2), Uj+(Un/2), Un/2, Vi,Vj,Vn/2); //Bpar( X21, U22, V11 )
        
        
        cilk_spawn Cpar(Xi,Xj,Xn/2, Ui, Uj+(Un/2), Un/2, Xi+(Xn/2),Xj,Xn/2); //Cpar( X11, U12, X21 )
        
        Cpar(Xi+(Xn/2),Xj+(Xn/2),Xn/2, Xi+(Xn/2),Xj,Xn/2, Vi,Vj+(Vn/2),Vn/2); //Cpar( X22, X21, V12 )
        
	cilk_sync;
        
        cilk_spawn Bpar(Xi,Xj,Xn/2, Ui,Uj,Un/2, Vi,Vj,Vn/2); //Bpar( X11, U11, V11 )
        
        Bpar(Xi+(Xn/2),Xj+(Xn/2),Xn/2, Ui+(Un/2),Uj+(Un/2),Un/2, Vi+(Vn/2),Vj+(Vn/2),Vn/2);//Bpar( X22, U22, V22 )
        
	cilk_sync;
        
        Cpar(Xi,Xj+(Xn/2),Xn/2,     Ui,Uj+(Un/2),Un/2, Xi+(Xn/2),Xj+(Xn/2),Xn/2); //Cpar( X12, U12, X22 )
        Cpar(Xi,Xj+(Xn/2),Xn/2,     Xi,Xj,Xn/2,     Vi,Vj+(Vn/2),Vn/2);//Cpar( X12, X11, V12 )
        
        Bpar(Xi,Xj+(Xn/2),Xn/2,     Ui, Uj,Un/2,    Vi+(Vn/2),Vj+(Vn/2),Vn/2);//Bpar( X12, U11, V22 )
        
        
    }
    else
    {
    //    Z[Xi][Xj] = mymin(Z[Xi][Xj],Z[Ui][Uj]+Z[Vi][Vj]);
//        SerPar(Ui,Uj,Xn*2);
        Bloopser(Xi,Xj,Xn,Ui,Uj,Vi,Vj);
    }
}

void Cpar(int Xi,int Xj, int Xn, int Ui, int Uj, int Un, int Vi,int Vj, int Vn)
{
    if(Xn>m)
    {
        cilk_spawn Cpar(Xi,Xj,Xn/2, Ui, Uj,Un/2, Vi,Vj,Vn/2);//Cpar( X11, U11, V11 )
        cilk_spawn Cpar(Xi,Xj+(Xn/2),Xn/2,     Ui,Uj,Un/2,     Vi,Vj+(Vn/2),Vn/2);//Cpar( X12, U11, V12 )
        cilk_spawn Cpar(Xi+(Xn/2),Xj,Xn/2, Ui+(Un/2), Uj, Un/2, Vi,Vj,Vn/2); //Cpar( X21, U21, V11 )
        Cpar(Xi+(Xn/2),Xj+(Xn/2),Xn/2, Ui+(Un/2),Uj,Un/2, Vi,Vj+(Vn/2),Vn/2); //Cpar( X22, U21, V12 )
        cilk_sync; 
        
        cilk_spawn Cpar(Xi,Xj,Xn/2, Ui,Uj+(Un/2),Un/2, Vi+(Vn/2),Vj,Vn/2);  //Cpar( X11, U12, V21 )
        cilk_spawn Cpar(Xi,Xj+(Xn/2),Xn/2,     Ui,Uj+(Un/2),Un/2,     Vi+(Vn/2),Vj+(Vn/2),Vn/2);//Cpar( X12, U12, V22 )
        cilk_spawn Cpar(Xi+(Xn/2),Xj,Xn/2, Ui+(Un/2),Uj+(Un/2),Un/2, Vi+(Vn/2),Vj,Vn/2); //Cpar( X21, U22, V21 )
        Cpar(Xi+(Xn/2),Xj+(Xn/2),Xn/2, Ui+(Un/2),Uj+(Un/2),Un/2, Vi+(Vn/2),Vj+(Vn/2),Vn/2); //Cpar( X22, U22, V22 )
        cilk_sync;
    } else
    {
//        Z[Xi][Xj] = min(Z[Xi][Xj],(Z[Ui][Uj] + Z[Vi][Vj]));
//        SerPar(Xi,Xj-(Xn),Xn*2);
        Cloopser(Xi,Xj,Xn,Ui,Uj,Vi,Vj);
    }
}

int mymin(int x, int y)
{
    if (x>y)
        return y;
    else
        return x;
}

void printZ(void)
{
    int row, columns;
    row = columns = COUNT;
    for (int row=0; row<COUNT; row++)
    {
        for(int columns=0; columns<COUNT; columns++)
            printf("%d      ", Z[row][columns]);
        printf("\n");
    }
}



void Aloopser(int Xi, int Xj, int Xn)
{
    int i,j,n,t,k;
    for (t = 2; t <= Xn-1; t++)
    {
        for(i=0;i<=Xn-t-1;i++)
        {
            j = t+i;
            for(k=i+1;k<=j;k++)
            {
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Xi+i][Xj+k]+Z[Xi+k][Xj+j]);
            }
        }
    }
}

void Bloopser(int Xi, int Xj, int Xn, int Ui, int Uj, int Vi, int Vj)
{
    int i,j,n,t,k;
    for (t = Xn-1; t >= 0; t--){
        for(i =t ; i < Xn; i++){
            j = i - t;
            for (k=i; k < Xn; k++){
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Ui+i][Uj+k]+Z[Vi+k][Vj+j]);
            }
            for (k =0; k <= j;k++) {
//X[i, j] ← min (X[i, j], U[i, k] + V [k, j] + w(xi + i, xj + j, uj))
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Ui+i][Uj+k]+Z[Vi+k][Vj+j]);
            }
        }
    }

/*for (int i = 0; i < Xn; i++) {
    int j = 0;
    int k = Xn - 1;
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Ui+i][Uj+k]+Z[Xi+Xn-1][Xj+j]);
}

for (int i = 0; i < Xn; i++) {
    for (int j = 1; j < Xn; j++) {
        for (int k = 0; k < Xn; k++) {
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Ui+i][Uj+k]+Z[Vi+k][Vj+j]);
        }
        for (int k = 0; k < j; k++) {
                Z[Xi+i][Xj+j]=min(Z[Xi+i][Xj+j],Z[Xi+i][Xj+k]+Z[Vi+k][Vj+j]);
        }
    }

}*/
}
void Cloopser(int Xi, int Xj, int Xn, int Ui, int Uj, int Vi, int Vj)
{
    int i,j,n,t,k;
    for (i = 0; i < Xn; i++)
        for(j =0 ; j < Xn; j++)
            for (k=0; k < Xn; k++)
//X[i, j] ← min (X[i, j], U[i, k] + V [k, j] + w(xi + i, xj + j, uj))
                Z[Xi+i][Xj+j] = min(Z[Xi+i][Xj+j],Z[Ui+i][Uj+k]+Z[Vi+k][Vj+j]);
}

void SerPar(int Xi,int Xj, int Xn)
{
    int i,j,k;
    if(Xn<2)
    {
        
    }
    else
    {
        for (i=0;i<=Xn-1;i++)
        {
            for (j=i+2;j<Xn;j++)
            {
                for (k=i;k<j+1;k++)
                {
                    Z[Xi+i][Xj+j] = min(Z[Xi+i][Xj+j],Z[Xi+i][Xj+k]+Z[Xi+k][Xj+j]);
                }
            }
        }
    }

}
