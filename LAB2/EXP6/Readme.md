# Experiment 6: False Sharing and Cache Line Contention in OpenMP

## Objective
Investigate the performance impact of **false sharing**, a hardware-level bottleneck that occurs when multiple threads update different variables residing on the same cache line, causing unnecessary cache coherence traffic.

## What is False Sharing?

**False sharing** occurs when:
- Multiple threads access different variables
- These variables happen to reside on the same cache line (typically 64 bytes)
- Cache coherence protocol forces invalidation across cores
- Performance degrades due to cache line "ping-ponging"

Even though threads work on **independent data**, they interfere with each other at the hardware level.

## Cache Line Basics
- **Cache Line Size**: 64 bytes (standard on most modern processors)
- **Problem**: If thread A writes to byte 0 and thread B writes to byte 8 of the same cache line, both cores must constantly invalidate and reload the entire line
- **Solution**: Pad data structures to ensure each thread's data is on a separate cache line

## Implementations

### Implementation 1: Using Structs (ques_6_1.c)

**Unpadded Structure:**
```c
typedef struct {
    double value;  // 8 bytes only
} UnpaddedData;
```

**Padded Structure:**
```c
typedef struct {
    double value;
    double padding[8];  // Force alignment to new cache line
} PaddedData;
```

**Results:**
| Configuration    | Time (s) | Speedup |
|------------------|----------|---------|
| False Sharing    | 1.229    | 1.00x   |
| Padded (Fixed)   | 0.381    | 3.23x   |

### Implementation 2: Using Arrays (ques_6_2.c)

**Unpadded Array:**
```c
int partial_sum_unpadded[NUM_THREADS];  // Adjacent in memory
```

**Padded Array:**
```c
int partial_sum_padded[NUM_THREADS][CP_SIZE];  // Each element on separate cache line
```

**Results:**
| Configuration | Time (s) | Speedup |
|---------------|----------|---------|
| Unpadded      | 1.698    | 1.00x   |
| Padded        | 0.332    | 5.11x   |

## Performance Analysis

### Implementation 1 (Struct-based)
- **Speedup**: 3.23x faster with padding
- **Impact**: 69% reduction in execution time
- **Overhead**: False sharing caused 222% slowdown

### Implementation 2 (Array-based)
- **Speedup**: 5.11x faster with padding
- **Impact**: 80% reduction in execution time
- **Overhead**: False sharing caused 411% slowdown

### Why Different Results?

**Array implementation shows worse false sharing** because:
- Integer array elements (4 bytes each) pack more tightly
- More thread data fits on a single 64-byte cache line
- Higher contention frequency
- More cache line invalidations per second

**Struct implementation has moderate false sharing** because:
- Double values (8 bytes each) are larger
- Fewer elements per cache line
- Lower (but still significant) contention

## Memory Layout Visualization

### Unpadded (False Sharing):
```
Cache Line 0: [T0][T1][T2][T3][T4][T5][T6][T7]
              ↑ All threads writing to same cache line!
              → Constant invalidation and coherence traffic
```

### Padded (No False Sharing):
```
Cache Line 0: [T0][----padding----]
Cache Line 1: [T1][----padding----]
Cache Line 2: [T2][----padding----]
...
              ↑ Each thread has its own cache line
              → No cross-core interference
```

## Key Findings

1. **False sharing can cause 3-5x performance degradation**
2. **Padding eliminates cache coherence overhead**
3. **Smaller data types suffer more** from false sharing
4. **Memory vs Speed trade-off**: Padding uses more memory but dramatically improves performance
5. **Hardware effect**: This is a CPU cache issue, not a software bug

## Cache Coherence Protocol Impact

When false sharing occurs:
1. Thread A writes to its variable → cache line marked as modified
2. Thread B writes to its variable on same cache line → invalidates Thread A's cache
3. Thread A needs data again → must fetch from Thread B's cache or memory
4. **Result**: Constant cache line transfers between cores

With padding:
1. Each thread owns its entire cache line
2. No invalidations from other threads
3. Cache line stays in modified state in local cache
4. **Result**: Maximum cache efficiency

## Best Practices

### When to Use Padding:
- High-frequency updates to thread-local data
- Performance-critical parallel loops
- Per-thread counters or accumulators
- Lock-free data structures

### When Padding May Not Help:
- Read-only shared data (no invalidations)
- Single-threaded code
- Memory-constrained environments
- Data already naturally aligned

## Compilation
```bash
gcc -fopenmp ques_6_1.c -o ques_6_1
gcc -fopenmp ques_6_2.c -o ques_6_2
```

## Visualizations

Generate performance comparison graphs:
```bash
python plot_ques6.py
```

## Conclusion

**False sharing is a critical performance consideration in parallel programming.** This experiment demonstrates that:

- Cache line contention can cause **3-5x performance loss**
- Simple padding techniques eliminate the problem
- The overhead varies based on data type size and access patterns
- **Always consider cache line alignment** when designing parallel data structures

**Rule of thumb**: If threads update separate variables frequently, ensure they're on separate cache lines (64-byte boundaries).
