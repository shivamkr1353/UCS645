CC = mpicc
CFLAGS = -Wall -O3

all: ring_comm array_sum max_min dot_product

ring_comm: ring_comm.c
	$(CC) $(CFLAGS) -o ring_comm ring_comm.c

array_sum: array_sum.c
	$(CC) $(CFLAGS) -o array_sum array_sum.c

max_min: max_min.c
	$(CC) $(CFLAGS) -o max_min max_min.c

dot_product: dot_product.c
	$(CC) $(CFLAGS) -o dot_product dot_product.c

clean:
	rm -f ring_comm array_sum max_min dot_product
