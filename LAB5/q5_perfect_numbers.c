#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VAL 10000

int is_perfect(int n) {
    if (n <= 1) return 0;
    int sum = 1;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            sum += i;
            if (i * i != n) {
                sum += n / i;
            }
        }
    }
    return sum == n;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Master process
        int current_num = 2;
        int active_slaves = size - 1;
        int msg;
        MPI_Status status;

        int perfects[MAX_VAL];
        int perfect_count = 0;

        while (active_slaves > 0) {
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            int slave_rank = status.MPI_SOURCE;

            if (msg > 0) { // It's a perfect number
                perfects[perfect_count++] = msg;
            }

            if (current_num <= MAX_VAL) {
                MPI_Send(&current_num, 1, MPI_INT, slave_rank, 0, MPI_COMM_WORLD);
                current_num++;
            } else {
                int terminate_signal = -1;
                MPI_Send(&terminate_signal, 1, MPI_INT, slave_rank, 0, MPI_COMM_WORLD);
                active_slaves--;
            }
        }

        printf("Found perfect numbers up to %d: ", MAX_VAL);
        for(int i = 0; i < perfect_count; i++) {
            printf("%d ", perfects[i]);
        }
        printf("\n");
    } else {
        // Slave process
        int msg = 0; // 0 indicates just starting
        MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        int num_to_test;
        while (1) {
            MPI_Recv(&num_to_test, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (num_to_test == -1) break; // Terminate signal

            if (is_perfect(num_to_test)) {
                msg = num_to_test;
            } else {
                msg = -num_to_test;
            }
            MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
