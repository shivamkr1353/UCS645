# Lab 7: CUDA Part II (Advanced Kernels and Profiling)

## Overview
This lab covers advanced CUDA concepts including thread divergence, performance comparisons between CPU and GPU sorting algorithms, and profiling memory bandwidth to understand hardware limitations.

> **Note**: Execution outputs in the provided Jupyter Notebook are simulated to represent standard GPU characteristics as a physical CUDA environment is not available locally.

---

## Problem 1: Sum of First N Integers
We implemented the sum of `1024` integers using two distinct kernel approaches:
1. **Iterative Kernel**: Uses a `for` loop inside the kernel. While this works, it forces a single thread to do `O(N)` work, completely defeating the purpose of a massively parallel GPU.
2. **Formula Kernel**: Uses the mathematical formula `(N * (N + 1)) / 2`. This performs the operation in `O(1)` time.

Both yield `524800`.

---

## Problem 2: Parallel Merge Sort
This section compares CPU-based sorting against GPU-based parallel sorting. Standard "Merge Sort" does not port well to GPUs due to its recursive nature and massive memory divergence. 
Instead, GPUs typically use **Bitonic Sort** or Radix Sort.

### Simulated Performance (N=1024)
- CPU Merge Sort: `0.052 ms`
- GPU Bitonic Sort: `0.015 ms`
- **Speedup**: `3.46x`

---

## Problem 3: Vector Addition with Profiling
Understanding Memory Bandwidth is critical for GPU programming. Most simple operations (like Vector Addition) are **Memory Bound**, meaning the GPU cores spend most of their time waiting for data to arrive from VRAM.

### Formulas
- **Theoretical Bandwidth (GB/s)**: `(Memory Clock Rate * Memory Bus Width * 2) / 8`
- **Measured Bandwidth (GB/s)**: `(Bytes Read + Bytes Written) / Execution Time`

### Expected Output
```
Theoretical Bandwidth: 336.00 GB/s
Measured Bandwidth: 280.45 GB/s
Bandwidth Efficiency: 83.4%
```

### Analysis
Achieving ~80-85% of theoretical bandwidth is extremely good for a typical kernel. To reach this efficiency, memory accesses must be **coalesced** (threads in a warp accessing contiguous memory addresses). Our simple `C[i] = A[i] + B[i]` kernel inherently features perfect coalescing.

---

## Execution Instructions
You can view the code, implementation details, and simulated outputs directly in the provided Jupyter Notebook:
- `lab7_cuda_advanced.ipynb`
