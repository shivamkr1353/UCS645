#include <omp.h>
#include <stdio.h>

#define NUM_THREADS 8
#define ITERATIONS 100000000
#define CP_SIZE (64 / sizeof(int))

int main() {
    int partial_sum_unpadded[NUM_THREADS] = {0};
    int partial_sum_padded[NUM_THREADS][CP_SIZE] = {{0}};

    double s1 = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        for(int i=0; i<ITERATIONS; i++) partial_sum_unpadded[tid]++;
    }
    double e1 = omp_get_wtime();

    double s2 = omp_get_wtime();
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int tid = omp_get_thread_num();
        for(int i=0; i<ITERATIONS; i++) partial_sum_padded[tid][0]++;
    }
    double e2 = omp_get_wtime();

    printf("Unpadded Time: %.6fs\n", e1 - s1);
    printf("Padded Time:   %.6fs\n", e2 - s2);

    return 0;
}