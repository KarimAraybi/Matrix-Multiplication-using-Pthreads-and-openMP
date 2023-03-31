#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

int main() {
    double **A, **B, **C;
    int i, j, k;
    A = (double **) malloc(N * sizeof(double *));
    B = (double **) malloc(N * sizeof(double *));
    C = (double **) malloc(N * sizeof(double *));
    for (i = 0; i < N; i++) {
        A[i] = (double *) malloc(N * sizeof(double));
        B[i] = (double *) malloc(N * sizeof(double));
        C[i] = (double *) malloc(N * sizeof(double));
    }

    srand(time(NULL));
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = rand() % 10+1;
            B[i][j] = rand() % 10+1;
            C[i][j] = 0;
        }
    }
    clock_t start = clock();
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    clock_t end = clock();
    double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Matrix multiplication took %f seconds\n", time_taken);


    free(A);
    free(B);
    free(C);

    return 0;
}