#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void test_sync_methods(long long N, int n_threads) {
    double sum_critical = 0.0;
    double sum_reduction = 0.0;

    omp_set_num_threads(n_threads);

    double start = omp_get_wtime();
    #pragma omp parallel for
    for (long long i = 0; i < N; i++) {
        #pragma omp critical
        {
            sum_critical += 1.0;
        }
    }
    double end = omp_get_wtime();
    double time_critical = end - start;

    start = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum_reduction)
    for (long long i = 0; i < N; i++) {
        sum_reduction += 1.0;
    }
    end = omp_get_wtime();
    double time_reduction = end - start;

    printf("%-8d %-20.6f %-20.6f", n_threads, time_critical, time_reduction);

    if (time_reduction > 0) {
        printf("%.2fx\n", time_critical / time_reduction);
    } else {
        printf("N/A\n");
    }
}

int main() {
    long long N = 10000000;

    printf("Comparing Critical Section vs Reduction with different thread counts\n");
    printf("Problem Size: %lld operations\n\n", N);
    printf("%-8s %-20s %-20s %s\n", "Threads", "Critical (s)", "Reduction (s)", "Overhead Factor");
    printf("------------------------------------------------------------------------\n");

    for (int threads = 1; threads <= 17; threads++) {
        test_sync_methods(N, threads);
    }

    return 0;
}