#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 65536 // 2^16

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double a = 2.5;
    double *X = NULL;
    double *Y = NULL;
    int local_n = N / size;

    double *local_X = (double*)malloc(local_n * sizeof(double));
    double *local_Y = (double*)malloc(local_n * sizeof(double));

    if (rank == 0) {
        X = (double*)malloc(N * sizeof(double));
        Y = (double*)malloc(N * sizeof(double));
        for (int i = 0; i < N; i++) {
            X[i] = i * 1.0;
            Y[i] = i * 2.0;
        }
    }

    double start_time = MPI_Wtime();

    MPI_Scatter(X, local_n, MPI_DOUBLE, local_X, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(Y, local_n, MPI_DOUBLE, local_Y, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        local_X[i] = a * local_X[i] + local_Y[i];
    }

    MPI_Gather(local_X, local_n, MPI_DOUBLE, X, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Process 0: Execution time = %f seconds\n", end_time - start_time);
        free(X);
        free(Y);
    }

    free(local_X);
    free(local_Y);

    MPI_Finalize();
    return 0;
}
