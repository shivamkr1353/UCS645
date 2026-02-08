#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    long long N = 100000000; 
    double *A = (double*)calloc(N, sizeof(double));
    double *B = (double*)malloc(N * sizeof(double));
    double *C = (double*)malloc(N * sizeof(double));
    double scalar = 3.3;

    for (long long i = 0; i < N; i++) {
        B[i] = 1.1;
        C[i] = 2.2;
    }

    int max_threads = 17;

    printf("Memory Bandwidth & Scalability Test (Triad Kernel)\n");
    printf("%-10s %-15s %-15s %s\n", "Cores", "Time (s)", "BW (GB/s)", "Speedup");
    printf("-------------------------------------------------------\n");

    double t_serial = 0;

    for (int threads = 1; threads <= max_threads; threads++) {
        double start = omp_get_wtime();

        #pragma omp parallel for num_threads(threads)
        for (long long i = 0; i < N; i++) {
            A[i] = B[i] + scalar * C[i];
        }

        double end = omp_get_wtime();
        double Tp = end - start;

        if (threads == 1) t_serial = Tp;

        double total_data_gb = (3.0 * N * sizeof(double)) / 1e9;
        double BW = total_data_gb / Tp;

        printf("%-10d %-15.6f %-15.2f %.2fx\n", threads, Tp, BW, t_serial / Tp);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}