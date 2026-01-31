#include <stdio.h>
#include <omp.h>

int main() {
    int i;
    
    printf("=== STATIC Scheduling (chunk size 4) ===\n");
    #pragma omp parallel for schedule(static, 4) num_threads(4)
    for (i = 0; i < 16; i++) {
        printf("Thread %d: iteration %d\n", omp_get_thread_num(), i);
    }
    
    printf("\n=== DYNAMIC Scheduling (chunk size 2) ===\n");
    #pragma omp parallel for schedule(dynamic, 2) num_threads(4)
    for (i = 0; i < 16; i++) {
        printf("Thread %d: iteration %d\n", omp_get_thread_num(), i);
    }
    
    printf("\n=== GUIDED Scheduling ===\n");
    #pragma omp parallel for schedule(guided) num_threads(4)
    for (i = 0; i < 16; i++) {
        printf("Thread %d: iteration %d\n", omp_get_thread_num(), i);
    }
    
    return 0;
}