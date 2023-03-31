#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "omp.h"



void Matrix_init( double **matrix, int size )
{
  int i, j;
  for (i = 0;i< size; ++i) {
    for (j = 0;j< size; ++j) {
      matrix[i][j] = 1.0;
    }
  }
}

double ** MatrixAlloc(int size)
{
 
  double ** arr = (double **) malloc( size * sizeof(double*) );

double * value = (double*) malloc( size * size * sizeof(double) );

  int i;
  for (i = 0;i< size; ++i) {
    arr[i] = &value[i*size];
  }

  return arr;
}

void PrintMatrix( double **matrix, int size )
{
  int i, j;
  for (i = 0;i< size; ++i) {
    for (j = 0;j< size-1; ++j) {
      printf( "%lf, ", matrix[i][j]);
    }
    printf("%lf", matrix[i][j] );
    putchar('\n');
  }
}

int main( int argc, char *argv[] )
{
  double **Matrix1, **Matrix2, **Matrix3;
  int len, i, j, k, chunksize, nbrofThreads;
  double tot = 0;
  struct timeval start, end;
  double etime;

  if (argc != 3) {
    fprintf( stderr, "%s <matrix size> <number of thread>\n", argv[0] );
    return -1;
  }

 len= atoi( argv[1] );
  nbrofThreads = atoi( argv[2] );

  if (len % nbrofThreads != 0) {
    return -1;
  }
  omp_set_num_threads( nbrofThreads );
  chunksize=len/nbrofThreads;
  Matrix1 = MatrixAlloc(len);
  Matrix2 = MatrixAlloc(len);
  Matrix3 = MatrixAlloc(len);
  Matrix_init( Matrix1,len);
  Matrix_init( Matrix2,len);

  if (len<= 10 ) {
    printf( "Matrix 1:\n" );
    PrintMatrix( Matrix1,len);
    printf( "Matrix 2:\n" );
    PrintMatrix( Matrix2,len);
  }

  gettimeofday( &start, NULL );
  
#pragma omp parallel for shared(Matrix1, Matrix2, Matrix3, chunksize) \
  private(i,j,k,tot) schedule(static, chunksize)
  for (i = 0;i<len; ++i) { 
    for (j = 0;j<len; ++j) { 
      tot = 0; 
      for (k = 0; k< len; ++k) { 
	tot += Matrix1[i][k]*Matrix2[k][j];
      }
      Matrix3[i][j] = tot;
    }
  }
  gettimeofday( &end, NULL );
  if (len<= 10 ) {
    printf( "Matrix 3:\n" );
    PrintMatrix( Matrix3,len);
  }
  etime = (end.tv_sec - start.tv_sec) * 1000.0;
  etime += (end.tv_usec - start.tv_usec) / 1000.0;
  printf( "Number of ranks: 0\tNumber of threads: %d\tExecution time:%.3lf sec\n",nbrofThreads, etime/1000.0);

  return 0;
}