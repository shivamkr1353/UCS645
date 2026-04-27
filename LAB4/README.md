# Lab 4: Introduction to MPI

## Overview
This lab introduces Message Passing Interface (MPI) for distributed-memory parallel computing. The focus is on basic point-to-point communication, collective communication operations (Scatter, Gather, Reduce), and understanding basic performance metrics.

---

## Exercise 1: Ring Communication

### Objective
Implement point-to-point communication where each process sends a message to the next process in a ring topology, accumulating values along the way.

### Implementation Details
- Process 0 starts with an initial value of `100`.
- Each process receives the value, adds its own rank to it, and sends it to the next process: `next_rank = (rank + 1) % size`.
- Finally, the value returns to Process 0.

### Expected Output (4 Processes)
```
Process 0 started with value 100
Process 1 received value 100
Process 2 received value 101
Process 3 received value 103
Process 0 received final value 106 from process 3
```

---

## Exercise 2: Parallel Array Sum

### Objective
Compute the sum of a 100-element array in parallel using MPI collective operations.

### Implementation Details
- Process 0 creates an array of size 100 with values `1` to `100`.
- `MPI_Scatter` is used to distribute chunks of the array to all processes.
- Each process computes its local sum.
- `MPI_Reduce` computes the global sum.
- Process 0 prints the sum and average.

### Expected Output (4 Processes)
```
Total sum: 5050
Average: 50.50
```

---

## Exercise 3: Finding Maximum and Minimum

### Objective
Generate random numbers on each process and find the global maximum and minimum values across all processes using MPI reductions.

### Implementation Details
- Each process generates 10 random numbers between 0 and 1000.
- Each process determines its local maximum and minimum.
- `MPI_Reduce` with `MPI_MAXLOC` and `MPI_MINLOC` is used to find the global extrema and the ranks of the processes that found them.

### Expected Output (4 Processes)
```
Process 0 generated: 45 892 120 40 5 999 300 250 88 12 
Process 1 generated: 345 12 90 44 8 88 234 1000 3 45 
Process 2 generated: 90 200 456 123 567 890 1 345 67 89 
Process 3 generated: 11 22 33 44 55 66 77 88 99 10 
Global Maximum: 1000 (found by process 1)
Global Minimum: 1 (found by process 2)
```

---

## Exercise 4: Parallel Dot Product

### Objective
Compute the dot product of two vectors in parallel.

### Implementation Details
- Vector A = `[1, 2, 3, 4, 5, 6, 7, 8]`
- Vector B = `[8, 7, 6, 5, 4, 3, 2, 1]`
- `MPI_Scatter` is used to distribute equal portions of both vectors to all processes.
- Each process computes the partial dot product of its local chunk.
- `MPI_Reduce` is used to sum the partial products into the final dot product.

### Expected Output (4 Processes)
```
Dot Product: 120
```

---

## Compilation and Execution

### Prerequisites
- MPICH or OpenMPI installed
- GCC compiler

### Build
```bash
make
```

### Run
```bash
mpirun -np 4 ./ring_comm
mpirun -np 4 ./array_sum
mpirun -np 4 ./max_min
mpirun -np 4 ./dot_product
```

## Key Learnings
1. **Point-to-Point vs Collective**: `MPI_Send`/`MPI_Recv` are flexible but require careful management to avoid deadlocks. Collective operations like `MPI_Scatter` and `MPI_Reduce` simplify code and are highly optimized.
2. **Data Distribution**: Memory is distributed. Only the root process needs to allocate memory for the full array when using scatter operations.
3. **Advanced Reductions**: Operations like `MPI_MAXLOC` are incredibly useful because they return not just the value, but also the rank of the process that found the value.
