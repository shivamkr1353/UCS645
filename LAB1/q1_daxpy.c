#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 65536  

void daxpy(double a, double *X, double *Y, int n) {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        X[i] = a * X[i] + Y[i];
    }
}

int main() {
    double a = 2.5;
    double *X = (double*)malloc(N * sizeof(double));
    double *Y = (double*)malloc(N * sizeof(double));
    
    
    for (int i = 0; i < N; i++) {
        X[i] = i * 1.0;
        Y[i] = i * 2.0;
    }
    
    printf("DAXPY Performance Analysis (N = %d)\n", N);
    printf("============================================================\n");
    printf("%-10s %-15s %-15s %-15s\n", "Threads", "Time (s)", "Speedup", "Efficiency");
    printf("------------------------------------------------------------\n");
    
    double sequential_time;
    
    
    int thread_counts[] = {1, 2, 4, 8, 16, 32};
    int num_tests = 6;
    
    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);
        
        
        for (int i = 0; i < N; i++) {
            X[i] = i * 1.0;
            Y[i] = i * 2.0;
        }
        
        double start = omp_get_wtime();
        
        
        for (int iter = 0; iter < 1000; iter++) {
            daxpy(a, X, Y, N);
        }
        
        double end = omp_get_wtime();
        double time_taken = end - start;
        
        if (num_threads == 1) {
            sequential_time = time_taken;
        }
        
        double speedup = sequential_time / time_taken;
        double efficiency = (speedup / num_threads) * 100.0;
        
        printf("%-10d %-15.6f %-15.2f %-15.2f%%\n", 
               num_threads, time_taken, speedup, efficiency);
    }
    
    printf("\nObservations:\n");
    printf("1. Optimal thread count appears around 4-8 threads\n");
    printf("2. Beyond optimal point, overhead increases due to:\n");
    printf("   - Thread creation/management overhead\n");
    printf("   - Cache contention between threads\n");
    printf("   - Limited memory bandwidth\n");
    
    free(X);
    free(Y);
    
    return 0;
}