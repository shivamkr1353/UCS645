# UCS645 - Parallel and Distributed Computing
## Lab 1: OpenMP Performance Analysis

### Overview
This repository contains the implementation and performance analysis of three parallel computing problems using OpenMP:

1. **Q1: DAXPY** - Vector operation (Double precision A*X Plus Y)
2. **Q2: Matrix Multiplication** - Two implementations (1D and 2D threading)
3. **Q3: π Calculation** - Numerical integration using reduction



### Key Results

#### Q1: DAXPY Performance
- **Optimal threads:** 8
- **Best speedup:** 7.24x
- **Efficiency:** 90.5%
- **Bottleneck:** Memory bandwidth

#### Q2: Matrix Multiplication Comparison

**1D Threading:**
| Threads | Time (s) | Speedup | Efficiency | GFLOPS |
|---------|----------|---------|------------|--------|
| 1       | 6.76     | 1.00    | 100.00%    | 0.30   |
| 4       | 1.62     | 4.17    | 104.24%    | 1.23   |
| 8       | 0.93     | 7.25    | 90.61%     | 2.14   |
| 16      | 0.81     | 8.39    | 52.44%     | 2.48   |

**2D Threading:**
| Threads | Time (s) | Speedup | Efficiency | GFLOPS |
|---------|----------|---------|------------|--------|
| 1       | 6.49     | 1.00    | 100.00%    | 0.31   |
| 4       | 1.52     | 4.28    | 107.06%    | 1.32   |
| 8       | 0.98     | 6.62    | 82.76%     | 2.04   |
| 16      | 0.89     | 7.27    | 45.46%     | 2.24   |

**Winner:** 1D threading (better high-thread scalability)

#### Q3: π Calculation
- **Best speedup:** 8.59x (32 threads)
- **Accuracy:** 10 decimal places
- **Computed π:** 3.141592653590426
- **Actual π:** 3.141592653589793
- **Error:** 6.33 × 10⁻¹⁰


### Performance Summary

| Problem | Type | Speedup | Scalability |
|---------|------|---------|-------------|
| DAXPY | Memory-bound | 8.20x | Limited |
| Matrix 1D | Memory-bound | 8.39x | Moderate |
| Matrix 2D | Memory-bound | 7.27x | Moderate |
| π Calc | Compute-bound | 8.59x | Excellent |
