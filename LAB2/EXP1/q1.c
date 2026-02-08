#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    long N = 100000000;

    double *A = (double*)malloc(N * sizeof(double));
    double *B = (double*)malloc(N * sizeof(double));
    double *C = (double*)malloc(N * sizeof(double));

    for (long i = 0; i < N; i++) {
        A[i] = B[i] = 1.0;
    }

    for (int threads = 1; threads <= 17; threads++) {
        omp_set_num_threads(threads);
        printf("Running with %d threads\n", threads);

        double start = omp_get_wtime();

        #pragma omp parallel for
        for (long i = 0; i < N; i++) {
            C[i] = A[i] + B[i];
        }

        double end = omp_get_wtime();

        printf("Time = %f seconds\n\n", end - start);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}