#include <stdio.h>
#include <omp.h>

#define NUM_STEPS 100000000  

double calculate_pi_sequential(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;
    
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    return step * sum;
}

double calculate_pi_parallel(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double sum = 0.0;
    
    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    return step * sum;
}

int main() {
    printf("Calculation of π using Numerical Integration\n");
    printf("Number of steps: %ld\n", NUM_STEPS);
    printf("======================================================================\n");
    
    
    printf("\nSequential Calculation:\n");
    double start_seq = omp_get_wtime();
    double pi_seq = calculate_pi_sequential(NUM_STEPS);
    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;
    
    printf("Pi value: %.15f\n", pi_seq);
    printf("Time: %.6f seconds\n", time_seq);
    
    printf("\n%-10s %-15s %-15s %-15s %-20s\n", 
           "Threads", "Pi Value", "Time (s)", "Speedup", "Efficiency");
    printf("----------------------------------------------------------------------\n");
    
   
    int thread_counts[] = {2, 4, 8, 16, 32};
    int num_tests = 5;
    
    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        omp_set_num_threads(num_threads);
        
        double start = omp_get_wtime();
        double pi = calculate_pi_parallel(NUM_STEPS);
        double end = omp_get_wtime();
        
        double time_taken = end - start;
        double speedup = time_seq / time_taken;
        double efficiency = (speedup / num_threads) * 100.0;
        
        printf("%-10d %-15.10f %-15.6f %-15.2f %-20.2f%%\n", 
               num_threads, pi, time_taken, speedup, efficiency);
    }
    
    printf("\nActual value of π: 3.14159265358979\n");
    printf("\nKey Points:\n");
    printf("1. reduction(+:sum) prevents race condition\n");
    printf("2. Each thread calculates partial sum independently\n");
    printf("3. OpenMP combines all partial sums at the end\n");
    printf("4. Speedup increases with threads but plateaus due to overhead\n");
    
    return 0;
}