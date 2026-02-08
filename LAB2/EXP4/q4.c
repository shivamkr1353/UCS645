#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void work(int i) {
    double dummy = 0;
    long long limit = (i + 1) * 100000; 
    for (long long j = 0; j < limit; ++j) {
        dummy += sin(j) * cos(j);
    }
}

void measure_imbalance(const char* schedule_name, int n_threads, int N) {
    double *thread_times = (double*)calloc(n_threads, sizeof(double));
    double start_wall = omp_get_wtime();
    
    #pragma omp parallel num_threads(n_threads)
    {
        int tid = omp_get_thread_num();
        double start_thread = omp_get_wtime();
        
        if (strcmp(schedule_name, "static") == 0) {
            #pragma omp for schedule(static)
            for (int i = 0; i < N; i++) work(i);
        } else if (strcmp(schedule_name, "dynamic,4") == 0) {
            #pragma omp for schedule(dynamic, 4)
            for (int i = 0; i < N; i++) work(i);
        } else if (strcmp(schedule_name, "guided") == 0) {
            #pragma omp for schedule(guided)
            for (int i = 0; i < N; i++) work(i);
        }
        
        double end_thread = omp_get_wtime();
        thread_times[tid] = end_thread - start_thread;
    }
    
    double end_wall = omp_get_wtime();
    double wall_time = end_wall - start_wall;
    
    double t_max = 0.0;
    double t_sum = 0.0;
    for (int i = 0; i < n_threads; i++) {
        if (thread_times[i] > t_max) t_max = thread_times[i];
        t_sum += thread_times[i];
    }
    double t_avg = t_sum / n_threads;
    double imbalance = (t_max - t_avg) / t_avg;
    
    printf("%-15s %-12.4fs%-12.4fs%.2f%%\n", 
           schedule_name, t_max, t_avg, imbalance * 100);
    
    free(thread_times);
}

int main() {
    int N = 2000;
    int n_threads = omp_get_max_threads();
    
    printf("Running on %d threads...\n", n_threads);
    printf("%-15s %-13s%-13s%s\n", "Schedule", "T_max", "T_avg", "Imbalance (%)");
    printf("-------------------------------------------------------\n");
    
    measure_imbalance("static", n_threads, N);
    measure_imbalance("dynamic,4", n_threads, N);
    measure_imbalance("guided", n_threads, N);
    
    return 0;
}