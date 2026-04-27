#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *array = NULL;
    int array_size = 100;
    int local_size = array_size / size;
    
    // Allocate memory for local array
    int *local_array = (int*)malloc(local_size * sizeof(int));

    if (rank == 0) {
        array = (int*)malloc(array_size * sizeof(int));
        for (int i = 0; i < array_size; i++) {
            array[i] = i + 1;
        }
    }

    // Distribute portions of the array
    MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < local_size; i++) {
        local_sum += local_array[i];
    }

    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total sum: %d\n", global_sum);
        printf("Average: %.2f\n", (double)global_sum / array_size);
        free(array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
