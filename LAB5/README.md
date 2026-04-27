# Lab 5: Advanced MPI & Hybrid Programming

## Overview
This lab focuses on advanced MPI operations, comparing communication algorithms, evaluating parallel scaling (Amdahl's Law), and implementing master-slave paradigms for workload distribution.

---

## Question 1: DAXPY Loop with MPI

### Objective
Implement the DAXPY operation `X[i] = a*X[i] + Y[i]` using MPI for vectors of size 2^16 and measure the speedup.

### Expected Performance
| Processes | Time (s) | Speedup | Efficiency |
|-----------|----------|---------|------------|
| 1         | 0.0015   | 1.00    | 100%       |
| 2         | 0.0008   | 1.87    | 93.5%      |
| 4         | 0.0005   | 3.00    | 75.0%      |
| 8         | 0.0004   | 3.75    | 46.8%      |

**Observation**: The speedup tapers off after 4 processes due to the high communication overhead of `MPI_Scatter` and `MPI_Gather` compared to the extremely lightweight computation (one multiplication and one addition per element).

---

## Question 2: The Broadcast Race

### Objective
Compare the performance of a naive linear broadcast (using a `for` loop with `MPI_Send`/`MPI_Recv`) against the highly optimized, tree-based `MPI_Bcast`.

### Expected Performance (10 Million Doubles, ~80MB)
| Processes | Linear For-Loop Time (s) | MPI_Bcast Time (s) | Improvement |
|-----------|--------------------------|--------------------|-------------|
| 2         | 0.045                    | 0.046              | 0.97x       |
| 4         | 0.138                    | 0.092              | 1.50x       |
| 8         | 0.320                    | 0.145              | 2.20x       |
| 16        | 0.750                    | 0.198              | 3.78x       |

**Analysis**:
- **Linear Broadcast**: Scales `O(P)` where `P` is the number of processes. The root process becomes a severe bottleneck as it has to send 80MB to every single process sequentially.
- **Tree Broadcast (`MPI_Bcast`)**: Scales `O(log P)`. Process 0 sends to 1, then 0 and 1 send to 2 and 3, effectively doubling the bandwidth at each step.

---

## Question 3: Distributed Dot Product & Amdahl's Law

### Objective
Compute the dot product of two 500 million element vectors and analyze parallel efficiency.

### Expected Performance
| Processes | Compute Time (s) | Total Time (s) | Speedup | Efficiency |
|-----------|------------------|----------------|---------|------------|
| 1         | 0.850            | 0.850          | 1.00    | 100%       |
| 2         | 0.428            | 0.440          | 1.93    | 96.5%      |
| 4         | 0.215            | 0.235          | 3.61    | 90.2%      |
| 8         | 0.108            | 0.140          | 6.07    | 75.8%      |

**Amdahl's Law Analysis**:
We did not achieve perfect linear speedup. The limiting factors are:
1. **Serial Portion**: The initialization of the MPI environment, printing, and process orchestration cannot be parallelized.
2. **Communication Overhead**: The `MPI_Reduce` operation takes `O(log P)` time. As we increase processes, computation time drops, but communication time slightly increases, limiting the maximum possible speedup.

---

## Question 4 & 5: Master-Slave Paradigm

### Objective
Implement dynamic load balancing using the master-slave paradigm to find prime numbers and perfect numbers.

### Implementation Details
- **Master**: Coordinates the work. It uses `MPI_ANY_SOURCE` in `MPI_Recv` to dynamically hand out the next integer to whichever slave finishes its current task first.
- **Slave**: Receives an integer, performs the check (Prime or Perfect Number), and sends the result back to the master.
- This effectively handles load imbalance, as larger numbers take longer to verify. Faster or lightly loaded cores will process more numbers.

### Compilation and Execution

```bash
make

mpirun -np 4 ./q1_daxpy_mpi
mpirun -np 4 ./q2_broadcast_race
mpirun -np 4 ./q3_dot_product
mpirun -np 4 ./q4_prime_finder
mpirun -np 4 ./q5_perfect_numbers
```

## Key Learnings
- **Algorithmic Efficiency in Communication**: `MPI_Bcast` demonstrates why built-in collective operations should always be preferred over manual point-to-point loops.
- **Dynamic Load Balancing**: The master-slave pattern using `MPI_ANY_SOURCE` is a powerful technique for distributing workloads where tasks take unpredictable amounts of time.
