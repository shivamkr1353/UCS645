CC = mpicc
CFLAGS = -Wall -O3

all: q1_daxpy_mpi q2_broadcast_race q3_dot_product q4_prime_finder q5_perfect_numbers

q1_daxpy_mpi: q1_daxpy_mpi.c
	$(CC) $(CFLAGS) -o q1_daxpy_mpi q1_daxpy_mpi.c

q2_broadcast_race: q2_broadcast_race.c
	$(CC) $(CFLAGS) -o q2_broadcast_race q2_broadcast_race.c

q3_dot_product: q3_dot_product.c
	$(CC) $(CFLAGS) -o q3_dot_product q3_dot_product.c

q4_prime_finder: q4_prime_finder.c
	$(CC) $(CFLAGS) -o q4_prime_finder q4_prime_finder.c

q5_perfect_numbers: q5_perfect_numbers.c
	$(CC) $(CFLAGS) -o q5_perfect_numbers q5_perfect_numbers.c

clean:
	rm -f q1_daxpy_mpi q2_broadcast_race q3_dot_product q4_prime_finder q5_perfect_numbers
