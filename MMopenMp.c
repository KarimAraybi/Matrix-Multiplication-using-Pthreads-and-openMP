#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include "omp.h"

int m,nbrofThreads,chunk;
int **Matrix1,**Matrix2,**MatrixF;

int main(int argc, char** argv)
{     
	struct timeval start, end;
  double etime;
  int i,x,z;
    m =  atoi(argv[1]);
    nbrofThreads = atoi(argv[2]);
    int * val1 = (int*) malloc( m * m * sizeof(int) );
	Matrix1 = (int**) malloc( m * sizeof(int*) );
	int * val2 = (int*) malloc( m * m * sizeof(int) );
    Matrix2 = (int**) malloc( m * sizeof(int*) );
	int * val3 = (int*) malloc( m * m * sizeof(int) );
    MatrixF = (int**) malloc( m * sizeof(int*) );
	
  for (int i = 0; i < m; ++i) {
    Matrix1[i] = &val1[i*m];
  }
  for (int i = 0; i < m; ++i) {
    Matrix2[ i ] = &val2[ i * m ];
  }
  for (int i = 0; i < m; ++i) {
    MatrixF[ i ] = &val3[ i * m ];
  }
    if (argc!=3){
        printf("err");
        return 1;
    }
    for (int i =0;i<m;i++){
        for (int j =0;j<m;j++){
            MatrixF[i][j]=0;
        }
    }
    for (int i =0;i<m;i++){
        for (int j =0;j<m;j++){
              Matrix1[i][j]= rand()%10+1;
         
        }
    } 
    for (int i =0;i<m;i++){
        for (int j =0;j<m;j++){
                 Matrix2[i][j]= rand()%10+1;
        }
    }
	
    omp_set_num_threads(nbrofThreads);
    chunk = m/nbrofThreads;
    gettimeofday( &start, NULL );

#pragma omp parallel for shared(Matrix1, Matrix2, MatrixF, chunk) \private(i,z,x) schedule(static, chunk)
    for(int x = 0; x<m;x++){
        for(int z = 0; z<m;z++){
            for (int i =0;i<m;i++){
                MatrixF[x][z]+=Matrix1[x][i]*Matrix2[i][z];
            }
        }
    }
   
  etime = (end.tv_sec - start.tv_sec) * 1000.0;
  etime = etime+ (end.tv_usec - start.tv_usec) / 1000.0;  

  printf( "Execution time:%.3lf sec\n", etime/1000.0);
        return 0;
}