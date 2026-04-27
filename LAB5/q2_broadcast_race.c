#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000000 // 10 million doubles (approx 80 MB)

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double *array = (double*)malloc(N * sizeof(double));

    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = 1.0;
        }
    }

    // Part A: MyBcast (Linear For-Loop)
    MPI_Barrier(MPI_COMM_WORLD);
    double start_mybcast = MPI_Wtime();

    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Send(array, N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(array, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_mybcast = MPI_Wtime();

    // Part B: MPI_Bcast (Tree Communication)
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = 2.0; // Reset
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_mpibcast = MPI_Wtime();

    MPI_Bcast(array, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_mpibcast = MPI_Wtime();

    if (rank == 0) {
        printf("Processes: %d\n", size);
        printf("MyBcast (Linear) Time: %f seconds\n", end_mybcast - start_mybcast);
        printf("MPI_Bcast (Tree) Time: %f seconds\n", end_mpibcast - start_mpibcast);
    }

    free(array);
    MPI_Finalize();
    return 0;
}
