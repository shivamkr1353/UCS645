#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct {
    int value;
    int rank;
} local_max, local_min, global_max, global_min;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    int numbers[10];
    local_max.value = -1;
    local_min.value = 1001;
    local_max.rank = rank;
    local_min.rank = rank;

    printf("Process %d generated: ", rank);
    for (int i = 0; i < 10; i++) {
        numbers[i] = rand() % 1001; // 0 to 1000
        printf("%d ", numbers[i]);
        if (numbers[i] > local_max.value) {
            local_max.value = numbers[i];
        }
        if (numbers[i] < local_min.value) {
            local_min.value = numbers[i];
        }
    }
    printf("\n");

    MPI_Reduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_min, &global_min, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Global Maximum: %d (found by process %d)\n", global_max.value, global_max.rank);
        printf("Global Minimum: %d (found by process %d)\n", global_min.value, global_min.rank);
    }

    MPI_Finalize();
    return 0;
}
