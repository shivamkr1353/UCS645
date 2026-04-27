#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 8;
    int local_n = n / size;

    int *A = NULL;
    int *B = NULL;
    int *local_A = (int*)malloc(local_n * sizeof(int));
    int *local_B = (int*)malloc(local_n * sizeof(int));

    if (rank == 0) {
        A = (int*)malloc(n * sizeof(int));
        B = (int*)malloc(n * sizeof(int));
        
        int init_A[] = {1, 2, 3, 4, 5, 6, 7, 8};
        int init_B[] = {8, 7, 6, 5, 4, 3, 2, 1};
        
        for (int i = 0; i < n; i++) {
            A[i] = init_A[i];
            B[i] = init_B[i];
        }
    }

    MPI_Scatter(A, local_n, MPI_INT, local_A, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_n, MPI_INT, local_B, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    int local_dot = 0;
    for (int i = 0; i < local_n; i++) {
        local_dot += local_A[i] * local_B[i];
    }

    int global_dot = 0;
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Dot Product: %d\n", global_dot);
        free(A);
        free(B);
    }

    free(local_A);
    free(local_B);

    MPI_Finalize();
    return 0;
}
