#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CACHE_LINE_SIZE 8

typedef struct {
    double value;
} UnpaddedData;

typedef struct {
    double value;
    double padding[CACHE_LINE_SIZE];
} PaddedData;

int main() {
    const long long iterations = 100000000;

    printf("Testing False Sharing vs Padding with 1-17 threads\n\n");
    printf("%-8s %-20s %-20s %s\n", "Threads", "False Sharing (s)", "Padded (s)", "Speedup");
    printf("------------------------------------------------------------------------\n");

    for (int n_threads = 1; n_threads <= 17; n_threads++) {
        UnpaddedData *unpadded = (UnpaddedData*)calloc(n_threads, sizeof(UnpaddedData));
        PaddedData *padded = (PaddedData*)calloc(n_threads, sizeof(PaddedData));

        double s1 = omp_get_wtime();
        #pragma omp parallel num_threads(n_threads)
        {
            int tid = omp_get_thread_num();
            for (long long i = 0; i < iterations; i++) {
                unpadded[tid].value += 1.0;
            }
        }
        double e1 = omp_get_wtime();
        double time_unpadded = e1 - s1;

        double s2 = omp_get_wtime();
        #pragma omp parallel num_threads(n_threads)
        {
            int tid = omp_get_thread_num();
            for (long long i = 0; i < iterations; i++) {
                padded[tid].value += 1.0;
            }
        }
        double e2 = omp_get_wtime();
        double time_padded = e2 - s2;

        printf("%-8d %-20.6f %-20.6f %.2fx\n",
               n_threads, time_unpadded, time_padded, time_unpadded / time_padded);

        free(unpadded);
        free(padded);
    }

    return 0;
}