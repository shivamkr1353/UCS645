# Experiment 1 — Execution Time Measurement (Vector Addition)

## Objective
Measure execution time of parallel vector addition using OpenMP with different thread counts (1-9 threads).

## Implementation
- **Vector Size**: 100,000,000 elements
- **Operation**: C[i] = A[i] + B[i]
- **Parallelization**: OpenMP `#pragma omp parallel for`

## Results

| Threads | Time (seconds) |
|---------|----------------|
| 1       | 0.479          |
| 2       | 0.137          |
| 3       | 0.106          |
| 4       | 0.092          |
| 5       | 0.106          |
| 6       | 0.099          |
| 7       | 0.096          |
| 8       | 0.093          |
| 9       | 0.087          |
| 10      | 0.104          |
| 11      | 0.091          |
| 12      | 0.089          |
| 13      | 0.097          |
| 14      | 0.092          |
| 15      | 0.081          |
| 16      | 0.094          |
| 17      | 0.091          |

## Analysis
- **Best Performance**: 15 threads (0.081s) - 5.91x speedup
- **Strong Scaling**: Significant improvement from 1-4 threads (0.479s → 0.092s)
- **Optimal Range**: 4-15 threads, with best at 15 threads
- **Key Insight**: Better scaling observed compared to previous run, showing 5.91x speedup at optimal thread count

## Compilation
```bash
gcc -fopenmp ques_1.c -o ques_1
```
