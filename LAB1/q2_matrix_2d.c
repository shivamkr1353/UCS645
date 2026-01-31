#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

void matrix_multiply_2d(double **A, double **B, double **C, int n) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

double** allocate_matrix(int n) {
    double **matrix = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (double*)malloc(n * sizeof(double));
    }
    return matrix;
}

void free_matrix(double **matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void initialize_matrix(double **matrix, int n) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (double)(i + j);
        }
    }
}

int main() {
    double **A = allocate_matrix(N);
    double **B = allocate_matrix(N);
    double **C = allocate_matrix(N);
    
    printf("Matrix Multiplication - 2D Threading (Size: %dx%d)\n", N, N);
    printf("======================================================================\n");
    
    // Initialize matrices
    double init_start = omp_get_wtime();
    initialize_matrix(A, N);
    initialize_matrix(B, N);
    double init_time = omp_get_wtime() - init_start;
    printf("Initialization time: %.4f seconds\n\n", init_time);
    
    printf("%-10s %-15s %-15s %-15s %-15s\n", 
           "Threads", "Time (s)", "Speedup", "Efficiency", "GFLOPS");
    printf("----------------------------------------------------------------------\n");
    
    double sequential_time;
    int thread_counts[] = {1, 2, 4, 8, 16};
    int num_tests = 5;
    
    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);
        
        double start = omp_get_wtime();
        matrix_multiply_2d(A, B, C, N);
        double end = omp_get_wtime();
        
        double time_taken = end - start;
        
        if (num_threads == 1) {
            sequential_time = time_taken;
        }
        
        double speedup = sequential_time / time_taken;
        double efficiency = (speedup / num_threads) * 100.0;
        
        // GFLOPS calculation
        double ops = 2.0 * N * N * N;
        double gflops = (ops / time_taken) / 1e9;
        
        printf("%-10d %-15.4f %-15.2f %-15.2f%% %-15.2f\n", 
               num_threads, time_taken, speedup, efficiency, gflops);
    }
    
    printf("\nVersion: 2D Threading (parallelized both i and j loops using collapse)\n");
    printf("Advantage: Better work distribution among threads\n");
    
    free_matrix(A, N);
    free_matrix(B, N);
    free_matrix(C, N);
    
    return 0;
}