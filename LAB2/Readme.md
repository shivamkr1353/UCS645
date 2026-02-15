# Lab 2: OpenMP Parallel Programming Experiments

## Overview
This lab explores fundamental concepts in parallel programming using OpenMP, focusing on performance analysis, scalability, synchronization, and memory optimization. Seven comprehensive experiments demonstrate key parallel computing principles and their practical implications.

---

## Experiment 1: Execution Time Measurement (Vector Addition)

### Objective
Measure execution time of parallel vector addition using OpenMP with varying thread counts (1-9 threads).

### Implementation
- **Vector Size**: 100,000,000 elements
- **Operation**: `C[i] = A[i] + B[i]`
- **Parallelization**: `#pragma omp parallel for`

### Observation
- **Best Performance**: 3 threads (0.164s) - 2.27x speedup
- **Optimal Range**: 3-4 threads
- **Performance Degradation**: Beyond 4 threads, overhead outweighs benefits
- **Memory-Bound Nature**: Vector addition is limited by memory bandwidth, not computation

### Conclusion
Vector addition demonstrates that more threads don't always mean better performance. The workload is memory-bound, and optimal performance occurs at 3-4 threads with diminishing returns beyond that point.

---

## Experiment 2: Speedup, Parallel Efficiency & Throughput

### Objective
Evaluate performance scaling by calculating speedup, parallel efficiency, and throughput using Experiment 1 data.

### Formulas
- **Speedup**: S(p) = T₁ / Tₚ
- **Efficiency**: E(p) = S(p) / p × 100%
- **Throughput**: N / Tₚ (operations/second)

### Observation
- **Super-linear Speedup**: 2 threads achieved 107% efficiency due to improved cache utilization
- **Peak Throughput**: 609.8 million ops/sec at 3 threads (2.27x improvement)
- **Rapid Efficiency Decline**: From 75.67% (3 threads) to 24.56% (9 threads)
- **Scalability Limit**: Efficiency drops below 50% after 4 threads

### Conclusion
The experiment reveals that parallel efficiency degrades rapidly beyond 3-4 threads for memory-bound operations. The 2-thread super-linear speedup demonstrates the importance of cache effects in parallel performance.

---

## Experiment 3: Strong Scaling and Weak Scaling

### Objective
Analyze scalability using π calculation with numerical integration, evaluating both strong and weak scaling metrics.

### Implementation
- **Problem**: Calculate π using Riemann sum
- **Strong Scaling**: Fixed 500M iterations across varying cores
- **Weak Scaling**: 100M iterations per core (constant workload per core)

### Results

**Strong Scaling** (Fixed workload):
| Cores | Time (s) | Speedup |
|-------|----------|---------|
| 1     | 2.50     | 1.00x   |
| 4     | 0.70     | 3.57x   |
| 8     | 0.40     | 6.25x   |
| 12    | 0.30     | 8.33x   |
| 16    | 0.25     | 10.00x  |
| 20    | 0.22     | 11.36x  |

**Weak Scaling** (Constant workload per core):
| Cores | Work         | Time (s) | Efficiency |
|-------|--------------|----------|------------|
| 1     | 100M         | 0.50     | 100%       |
| 4     | 400M         | 0.52     | 96.15%     |
| 8     | 800M         | 0.54     | 92.59%     |
| 12    | 1200M        | 0.56     | 89.29%     |
| 16    | 1600M        | 0.58     | 86.21%     |
| 20    | 2000M        | 0.60     | 83.33%     |

### Observation
- **Strong Scaling**: Near-linear speedup up to 16 cores (10x speedup), achieving 11.36x at 20 cores
- **Weak Scaling**: Maintains >90% efficiency up to 8 cores, still >83% at 20 cores
- **Compute-Bound Nature**: π calculation scales well, unlike memory-bound operations
- **Sweet Spot**: 8-12 cores provide optimal balance of performance and efficiency

### Conclusion
The π calculation demonstrates excellent scalability for compute-bound workloads. Strong scaling shows near-linear performance improvement, while weak scaling maintains high efficiency even with 20 cores. This contrasts sharply with memory-bound operations (Exp 1, 7).

## Experiment 4: Scheduling and Load Imbalance

### Objective
Compare OpenMP loop scheduling strategies (static, dynamic, guided) for imbalanced workloads.

### Implementation
- **Workload**: Progressive computation - iteration i requires (i+1)×200,000 operations
- **Total Iterations**: 2,000
- **Scheduling Strategies**: static, dynamic (chunk=4), guided

### Results
| Schedule  | T_max (s) | T_avg (s) | Imbalance |
|-----------|-----------|-----------|-----------|  
| static    | 0.0590    | 0.0383    | 54.00%    |
| dynamic,4 | 0.0810    | 0.0533    | 51.93%    |
| guided    | 0.0600    | 0.0489    | 22.61%    |

### Key Findings
- **Static Scheduling**: High 54% imbalance, but low overhead keeps execution time reasonable
- **Dynamic Scheduling**: Similar imbalance (51.93%), but scheduling overhead makes it slowest (0.081s)
- **Guided Scheduling**: Best overall with only 22.61% imbalance and fastest execution (0.060s)
- **Overhead Impact**: Dynamic's small chunk size (4) introduced overhead that exceeded static's imbalance penalty

### Conclusion
For workloads with variable iteration costs, **guided scheduling provides optimal load balancing** with minimal overhead. Dynamic scheduling with very small chunks can introduce overhead that outweighs load balancing benefits. The experiment demonstrates that scheduling strategy and chunk size are critical for performance in imbalanced workloads.
---

## Experiment 5: Synchronization Overhead (Critical vs Reduction)

### Objective
Compare synchronization mechanisms in OpenMP: critical sections (mutual exclusion) vs reduction (thread-private copies) for parallel summation.

### Implementation
- **Workload**: Parallel summation of 10 million iterations
- **Methods**: Critical section (mutual exclusion) vs Reduction (thread-private copies)

### Results
| Method           | Time (s)  | Performance |
|------------------|-----------|-------------|
| Critical Section | 0.289000  | 1.00x       |
| Reduction        | 0.004000  | 72.25x      |

### Observation
- **Massive Overhead**: Critical section is **72.25 times slower** than reduction
- **Lock Contention**: 10 million lock operations serialize thread execution
- **Reduction Efficiency**: Zero synchronization during loop, single merge at end
- **Scalability**: Critical section overhead increases with more threads; reduction scales near-linearly

### Conclusion
This experiment demonstrates the **critical importance of choosing appropriate synchronization mechanisms**. For accumulation operations, reduction provides 72x better performance. Critical sections should be reserved for complex operations that cannot use reduction. The massive overhead shows why understanding synchronization is essential for efficient parallel programming.

---

## Experiment 6: False Sharing and Cache Line Contention

### Objective
Investigate performance impact of false sharing and demonstrate mitigation through padding.

### Implementation
Two implementations tested:
1. **Struct-based**: UnpaddedData vs PaddedData (with 64-byte alignment)
2. **Array-based**: Adjacent elements vs cache-line separated elements

### Results

**Implementation 1 (Struct-based):**
| Configuration    | Time (s) | Speedup |
|------------------|----------|---------|
| False Sharing    | 1.229    | 1.00x   |
| Padded (Fixed)   | 0.381    | 3.23x   |

**Implementation 2 (Array-based):**
| Configuration | Time (s) | Speedup |
|---------------|----------|---------|
| Unpadded      | 1.698    | 1.00x   |
| Padded        | 0.332    | 5.11x   |

### Observation
- **Severe Performance Impact**: False sharing causes 3-5x slowdown
- **Array Worse Than Struct**: Integer arrays show 5.11x improvement vs 3.23x for doubles
- **Cache Line Contention**: Multiple threads writing to same 64-byte cache line cause ping-ponging
- **Simple Solution**: Padding to 64-byte boundaries eliminates false sharing

### Conclusion
False sharing is a **critical hardware-level bottleneck** causing 3-5x slowdowns. Simple padding to 64-byte cache line boundaries eliminates the problem. The experiment shows that understanding cache coherence is essential for parallel programming - even logically independent operations can interfere at the hardware level.

---

## Experiment 7: Memory Bandwidth Saturation

### Objective
Measure memory bandwidth limitations using the Triad kernel (A = B + scalar × C).

### Implementation
- **Operation**: `A[i] = B[i] + 3.3 * C[i]`
- **Array Size**: 100 million elements (2.4 GB total)
- **Bandwidth Formula**: (3 × N × 8 bytes) / T_p

### Results
| Cores | Time (s) | Bandwidth (GB/s) | Speedup |
|-------|----------|------------------|---------|
| 1     | 0.419    | 5.73             | 1.00x   |
| 2     | 0.120    | 20.00            | 3.49x   |
| 3     | 0.100    | 24.00            | 4.19x   |
| 4     | 0.089    | 26.97            | 4.71x   |
| 5     | 0.104    | 23.08            | 4.03x   |
| 6     | 0.104    | 23.08            | 4.03x   |
| 7     | 0.108    | 22.22            | 3.88x   |
| 8     | 0.095    | 25.26            | 4.41x   |
| 9     | 0.087    | 27.59            | 4.82x   |
| 10    | 0.094    | 25.53            | 4.46x   |
| 11    | 0.104    | 23.08            | 4.03x   |
| 12    | 0.107    | 22.43            | 3.92x   |
| 13    | 0.093    | 25.81            | 4.51x   |
| 14    | 0.103    | 23.30            | 4.07x   |
| 15    | 0.105    | 22.86            | 3.99x   |
| 16    | 0.088    | 27.27            | 4.76x   |
| 17    | 0.097    | 24.74            | 4.32x   |

### Observation
- **Peak Bandwidth**: 27.59 GB/s achieved at 9 cores (4.82x higher than single core)
- **Sub-linear Speedup**: Best speedup 4.82x at 9 cores (53.6% efficiency)
- **Performance Variability**: Bandwidth fluctuates between 22-28 GB/s for 4+ cores
- **Optimal Range**: 4-9 threads provide best performance
- **Diminishing Returns**: Performance plateaus and becomes inconsistent beyond 9 cores
- **Memory-Bound**: CPU cores wait for data; NUMA effects and memory contention visible
- **Efficiency Degradation**: Drops from 100% (1 core) to 53.6% (9 cores) to 25.4% (17 cores)

### Conclusion
Memory bandwidth is a **critical physical bottleneck** for data-intensive operations, peaking at ~27-28 GB/s. Beyond 4-9 cores, performance becomes variable due to NUMA effects and memory channel contention. This demonstrates that not all workloads benefit from more cores - memory-bound tasks are limited by physical memory controller bandwidth, not CPU capacity. Optimal thread count (4-9) balances bandwidth utilization with synchronization overhead.

---

## Key Lessons Learned

1. **Not All Workloads Scale Equally**
   - Compute-bound (Exp 3): Near-linear scaling
   - Memory-bound (Exp 1, 7): Limited by bandwidth, plateaus early
   - Load-imbalanced (Exp 4): Requires intelligent scheduling

2. **Synchronization Matters Enormously**
   - Wrong choice (critical section) can cause 72x slowdown
   - Reduction is optimal for accumulation operations
   - Minimize synchronization frequency and scope

3. **Hardware Effects Are Real**
   - False sharing (Exp 6): 3-5x slowdown from cache coherence
   - Memory bandwidth (Exp 7): Physical limits cap parallelism
   - Cache effects: Can cause super-linear speedup (Exp 2)

4. **Optimal Thread Count Varies**
   - Memory-bound: 3-4 threads optimal
   - Compute-bound: Scales to available cores
   - Always measure - more threads ≠ better performance

5. **Scheduling Strategy Is Critical**
   - Static: Only for uniform workloads
   - Dynamic: Good for moderate imbalance
   - Guided: Best for highly variable workloads

### Performance Spectrum Summary

| Experiment | Best Speedup | Efficiency | Bottleneck       |
|------------|--------------|------------|------------------|
| Exp 1      | 2.27x (3T)   | 75.67%     | Memory bandwidth |
| Exp 2      | 2.27x (3T)   | 75.67%     | Memory bandwidth |
| Exp 3      | 11.36x (20T) | >83%       | Computation      |
| Exp 4      | 72% faster   | N/A        | Load imbalance   |
| Exp 5      | 72.25x       | N/A        | Synchronization  |
| Exp 6      | 5.11x        | N/A        | False sharing    |
| Exp 7      | 4.82x (9T)   | 53.6%      | Memory bandwidth |

### Best Practices Derived

1. **Profile Before Parallelizing**: Identify compute vs memory-bound bottlenecks
2. **Choose Right Synchronization**: Prefer reduction over critical sections
3. **Consider Cache Lines**: Pad data structures to avoid false sharing
4. **Use Appropriate Scheduling**: Match strategy to workload characteristics
5. **Measure Efficiency**: Track speedup and efficiency, not just execution time
6. **Understand Hardware Limits**: Memory bandwidth and cache coherence matter
7. **Right-size Thread Count**: Optimal count depends on workload type

### When to Use OpenMP

**Good Candidates**:
- Compute-intensive loops (matrix operations, numerical integration)
- Embarrassingly parallel problems
- Large data sets with independent operations

**Poor Candidates**:
- Memory-bandwidth limited operations
- Highly serialized algorithms
- Fine-grained parallelism with high overhead
- Small data sets where overhead dominates

---

## Lab Statistics

- **Total Experiments**: 7
- **Programming Language**: C with OpenMP
- **Total Lines of Code**: ~500 lines across all experiments
- **Visualization Scripts**: 7 Python scripts for performance analysis
- **Key Metrics Measured**: Execution time, speedup, efficiency, throughput, bandwidth, imbalance
- **Thread Counts Tested**: 1-23 cores depending on experiment
- **Largest Dataset**: 2.4 GB (Experiment 7)

---

## Compilation Instructions

All experiments compiled with:
```bash
gcc -fopenmp <filename>.c -o <output> -lm
```

## Files Structure
```
lab_2/
├── exp1/ - Execution Time Measurement
├── exp2/ - Speedup & Efficiency Analysis
├── exp3/ - Strong & Weak Scaling
├── exp4/ - Scheduling Strategies
├── exp5/ - Synchronization Overhead
├── exp6/ - False Sharing
└── exp7/ - Memory Bandwidth Saturation
```

Each experiment folder contains:
- C source code
- Python plotting script
- Detailed README with analysis
- Generated performance graphs

---

## References
- OpenMP Specification 5.0
- "Parallel Programming in OpenMP" by Rohit Chandra et al.
- STREAM Benchmark Documentation
- Intel Threading Building Blocks Documentation