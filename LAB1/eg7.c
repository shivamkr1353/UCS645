#include <stdio.h>
#include <omp.h>

int main() {
    int counter = 0;
    
    printf("=== WITHOUT Critical Section (Race Condition) ===\n");
    #pragma omp parallel num_threads(4)
    {
        for(int i = 0; i < 1000; i++) {
            counter++; 
        }
    }
    printf("Counter (should be 4000): %d\n\n", counter);
    
    
    counter = 0;
    
    printf("=== WITH Critical Section (Safe) ===\n");
    #pragma omp parallel num_threads(4)
    {
        #pragma omp critical
        {
            printf("Thread %d entering critical section\n", 
                   omp_get_thread_num());
           
            for(int i = 0; i < 1000; i++) {
                counter++;
            }
            printf("Thread %d leaving critical section\n", 
                   omp_get_thread_num());
        }
    }
    printf("Counter (should be 4000): %d\n", counter);
    
    return 0;
}