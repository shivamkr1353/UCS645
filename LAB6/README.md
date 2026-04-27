# Lab 6: Introduction to CUDA

## Overview
This lab introduces GPU computing using the CUDA API. It covers how to query a GPU's physical limits, perform host-to-device and device-to-host memory transfers, write basic compute kernels (array summation), and calculate performance metrics for Matrix Addition.

> **Note**: As the local environment does not have a CUDA-enabled GPU configured, the execution outputs provided in the Jupyter Notebook are simulated to accurately reflect the architecture of a typical compute-capable GPU (e.g., Ampere architecture, Compute Capability 8.6).

---

## Contents

### 1. Device Query
The code uses `cudaGetDeviceCount` and `cudaGetDeviceProperties` to extract limits:
- Maximum block/grid dimensions
- Available Global, Shared, and Constant memory
- Warp size
- Compute Capability

### 2. Array Sum (Reduction)
This section demonstrates a fundamental parallel pattern: **Reduction**. 
- It uses `__shared__` memory so threads in the same block can collaboratively sum their values very quickly before writing the result back to global memory via `atomicAdd`.
- This avoids race conditions and severely reduces global memory traffic compared to having every thread use `atomicAdd` on the final sum variable.

### 3. Matrix Addition Analysis
The matrix addition kernel simply performs `C[index] = A[index] + B[index]`.
- **FLOPS**: For integer matrices, there are technically 0 FLOPS, but exactly `N^2` integer additions.
- **Reads**: `2 * N^2` (reading from A and B)
- **Writes**: `N^2` (writing to C)

---

## Execution Instructions
You can view the code, implementation details, and simulated outputs directly in the provided Jupyter Notebook:
- `lab6_cuda_intro.ipynb`
