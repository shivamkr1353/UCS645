#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int value;

    if (rank == 0) {
        value = 100;
        printf("Process 0 started with value %d\n", value);
        value += rank;
        MPI_Send(&value, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        // Receive the final value from the last process
        MPI_Recv(&value, 1, MPI_INT, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 0 received final value %d from process %d\n", value, size - 1);
    } else {
        MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received value %d\n", rank, value);
        value += rank;
        int next_rank = (rank + 1) % size;
        MPI_Send(&value, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
