#include <stdio.h>
#include <omp.h>

int main() {
    int x = 10; 
    
    printf("=== SHARED Variable (default) ===\n");
    printf("Initial x = %d\n\n", x);
    
    #pragma omp parallel num_threads(4)
    {
        printf("Thread %d sees x = %d\n", 
               omp_get_thread_num(), x);
        x = omp_get_thread_num(); 
        printf("Thread %d set x = %d\n", 
               omp_get_thread_num(), x);
    }
    printf("\nAfter parallel region: x = %d (unpredictable!)\n\n", x);
    
   
    x = 10;
    
    printf("=== PRIVATE Variable ===\n");
    printf("Initial x = %d\n\n", x);
    
    #pragma omp parallel private(x) num_threads(4)
    {
        
        printf("Thread %d: private x = %d (garbage value)\n", 
               omp_get_thread_num(), x);
        
        x = omp_get_thread_num(); 
        printf("Thread %d: set private x = %d\n", 
               omp_get_thread_num(), x);
    }
    printf("\nAfter parallel region: x = %d (unchanged!)\n", x);
    
    return 0;
}