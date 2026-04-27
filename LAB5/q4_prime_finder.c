#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_VAL 1000

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
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

        int primes[MAX_VAL];
        int prime_count = 0;

        while (active_slaves > 0) {
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            int slave_rank = status.MPI_SOURCE;

            if (msg > 0) { // It's a prime
                primes[prime_count++] = msg;
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

        printf("Found %d primes up to %d\n", prime_count, MAX_VAL);
    } else {
        // Slave process
        int msg = 0; // 0 indicates just starting
        MPI_Send(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        int num_to_test;
        while (1) {
            MPI_Recv(&num_to_test, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (num_to_test == -1) break; // Terminate signal

            if (is_prime(num_to_test)) {
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
