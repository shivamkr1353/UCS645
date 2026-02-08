# Experiment 7: Memory Bandwidth Saturation

## Objective
Measure the memory bandwidth limitations of a multicore processor using the **Triad kernel** operation and study how memory bandwidth and execution time scale with increasing thread count.

## The Triad Kernel Operation

The benchmark implements the following vector operation:
$$A[i] = B[i] + scalar \times C[i]$$

This is a **classical memory-bound benchmark** because:
- Minimal computation per data element (one multiply, one add)
- Large volume of data movement between memory and CPU
- Performance limited by memory bandwidth, not CPU speed

## Memory Bandwidth Formula

$$\text{Bandwidth (GB/s)} = \frac{3 \times N \times \text{sizeof(double)}}{T_p \times 10^9}$$

Where:
- **Factor 3**: Represents three memory operations (Read B, Read C, Write A)
- **N**: Number of array elements (100,000,000)
- **sizeof(double)**: 8 bytes
- **Tp**: Parallel execution time for each thread count

## Implementation
- **Array Size**: 100 million elements per array
- **Total Memory**: ~2.4 GB (3 arrays × 100M × 8 bytes)
- **Operation**: `A[i] = B[i] + 3.3 * C[i]`
- **Threads**: 1 to 8 cores

## Results

| Cores | Time (s) | BW (GB/s) | Speedup |
|-------|----------|-----------|---------|
| 1     | 0.838    | 2.86      | 1.00x   |
| 2     | 0.208    | 11.54     | 4.03x   |
| 3     | 0.174    | 13.79     | 4.82x   |
| 4     | 0.177    | 13.56     | 4.73x   |
| 5     | 0.171    | 14.04     | 4.90x   |
| 6     | 0.176    | 13.64     | 4.76x   |
| 7     | 0.173    | 13.87     | 4.84x   |
| 8     | 0.170    | 14.12     | 4.93x   |

## Performance Analysis

### Execution Time
- **Single Core**: 0.838 seconds
- **8 Cores**: 0.170 seconds
- **Time Reduction**: 80% decrease

### Memory Bandwidth
- **Single Core**: 2.86 GB/s (baseline)
- **Peak Bandwidth**: 14.12 GB/s at 8 cores
- **Bandwidth Increase**: 4.94x improvement
- **Saturation Point**: Bandwidth plateaus around 13.5-14.1 GB/s after 3 cores

### Speedup Analysis
- **Best Speedup**: 4.93x at 8 cores
- **Parallel Efficiency**: 61.6% (4.93/8 = 0.616)
- **Strong Scaling**: Good scaling from 1-3 cores, then plateaus

## Key Observations

### 1. Memory Bandwidth Saturation
- Bandwidth increases rapidly from 1-3 cores (2.86 → 13.79 GB/s)
- **Plateaus** at ~14 GB/s from 3-8 cores
- Adding more threads beyond 3 cores provides minimal bandwidth improvement
- System memory bandwidth limit reached at approximately **14 GB/s**

### 2. Speedup Limitation
- Speedup is **sub-linear** (4.93x with 8 cores instead of ideal 8x)
- Limited by memory bandwidth, not CPU computational capacity
- Efficiency drops from 100% (1 core) to 61.6% (8 cores)

### 3. Memory-Bound Behavior
- This workload is **memory-bound**, not compute-bound
- All cores share the same memory bus
- Memory controller becomes the bottleneck
- CPU cores are underutilized, waiting for data

### 4. Diminishing Returns
- **Strong scaling** from 1-2 cores: 4.03x speedup
- **Weak scaling** from 3-8 cores: Only 1.02x additional improvement
- Adding cores 5-8 provides less than 2% performance gain

## Performance Breakdown

| Metric              | 1→2 Cores | 2→3 Cores | 3→8 Cores |
|---------------------|-----------|-----------|-----------|
| Time Reduction      | 75.2%     | 16.3%     | 2.3%      |
| BW Increase         | 4.03x     | 1.19x     | 1.02x     |
| Speedup Gain        | 3.03x     | 0.79x     | 0.11x     |

## Why Memory Bandwidth Matters

For memory-intensive operations:
1. **Data Transfer Dominates**: Moving 2.4 GB of data takes longer than arithmetic
2. **Shared Resource**: All cores compete for memory bus access
3. **Bandwidth Ceiling**: Physical memory controller limit (~14 GB/s in this system)
4. **Amdahl's Law Effect**: Memory bandwidth is the sequential bottleneck

## Theoretical vs Actual Performance

**Theoretical Peak** (if memory unlimited):
- 8 cores should give 8x speedup

**Actual Performance**:
- Only 4.93x speedup due to memory bandwidth saturation

**Efficiency Loss**:
- 38.4% efficiency lost to memory bottleneck

## Comparison with Compute-Bound Tasks

| Aspect           | Memory-Bound (Triad) | Compute-Bound (Matrix Multiply) |
|------------------|----------------------|---------------------------------|
| Bottleneck       | Memory bandwidth     | CPU computation                 |
| Scaling          | Sub-linear (4.93x)   | Near-linear (7-8x)             |
| Peak Efficiency  | 61.6%                | >90%                           |
| Core Utilization | Low (waiting on data)| High (always computing)        |

## Visualizations

### Combined Analysis
![Triad Benchmark Results](triad_benchmark_results.png)

### Individual Metrics
![Execution Time vs Cores](execution_time_vs_cores.png)
![Memory Bandwidth vs Cores](bandwidth_vs_cores.png)
![Speedup vs Cores](speedup_vs_cores.png)

Generate all plots with:
```bash
python plot_ques7.py
```

## Implications for Parallel Programming

### Best Practices:
1. **Identify Bottleneck**: Determine if workload is memory or compute-bound
2. **Optimize Data Access**: Use cache-friendly algorithms for memory-bound tasks
3. **Right-size Thread Count**: More threads ≠ better performance for memory-bound work
4. **Data Locality**: Keep frequently accessed data in cache
5. **Consider Algorithms**: Sometimes sequential algorithms are faster for small data

### When to Expect Memory Saturation:
- Large array operations (like this experiment)
- Sparse matrix computations
- Graph algorithms with random memory access
- Video/image processing with large frames
- Database operations with large datasets

## Hardware Considerations

This system demonstrates:
- **Memory Bandwidth**: ~14 GB/s (typical for DDR4-2400/2666)
- **Cores**: 8 logical cores
- **Cache Hierarchy**: L3 cache insufficient for 2.4 GB dataset
- **Memory Controller**: Single-channel or dual-channel configuration

## Compilation
```bash
gcc -fopenmp ques_7.c -o ques_7
```

## Conclusion

This experiment demonstrates that **memory bandwidth is a critical limiting factor** in parallel performance. Key findings:

1. **Bandwidth saturates at ~14 GB/s** regardless of core count beyond 3
2. **Speedup limited to 4.93x** with 8 cores (61.6% efficiency)
3. **Adding cores 4-8 provides minimal benefit** (<3% improvement)
4. **Memory-bound workloads don't scale linearly** with core count

**Lesson**: Understanding whether your application is memory-bound or compute-bound is essential for effective parallelization. For memory-bound tasks, focus on optimizing data access patterns and cache utilization rather than simply adding more threads.
