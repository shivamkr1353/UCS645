#include <iostream>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <omp.h>

void correlate_seq(int ny, int nx, const float* data, float* result);
void correlate_omp(int ny, int nx, const float* data, float* result);
void correlate_fast(int ny, int nx, const float* data, float* result);

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: ./correlate <ny> <nx>\n";
        return 1;
    }

    int ny = std::atoi(argv[1]);
    int nx = std::atoi(argv[2]);

    float* data = new float[ny * nx];
    float* result = new float[ny * ny];

    // Fill matrix with random values
    for (int i = 0; i < ny * nx; i++)
        data[i] = static_cast<float>(rand()) / RAND_MAX;

    // Thread counts to test
    int thread_counts[] = {1, 2, 4, 8, 10};
    int num_tests = 5;

    // Store results
    double seq_time = 0.0;
    double omp_times[5];
    double fast_times[5];

    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "Parallel Correlation Matrix Computation - Performance Analysis\n";
    std::cout << "Matrix Size: " << ny << " x " << nx << "\n";
    std::cout << std::string(80, '=') << "\n\n";

    // Run Sequential (only once)
    std::cout << "Running Sequential (baseline)...\n";
    auto start = std::chrono::high_resolution_clock::now();
    correlate_seq(ny, nx, data, result);
    auto end = std::chrono::high_resolution_clock::now();
    seq_time = std::chrono::duration<double>(end - start).count();
    std::cout << "Sequential Time: " << seq_time << " sec\n\n";

    // Run OpenMP and Fast with different thread counts
    for (int i = 0; i < num_tests; i++) {
        int threads = thread_counts[i];
        omp_set_num_threads(threads);
        
        std::cout << "Testing with " << threads << " thread(s)...\n";
        
        // OpenMP version
        start = std::chrono::high_resolution_clock::now();
        correlate_omp(ny, nx, data, result);
        end = std::chrono::high_resolution_clock::now();
        omp_times[i] = std::chrono::duration<double>(end - start).count();
        
        // Fast version
        start = std::chrono::high_resolution_clock::now();
        correlate_fast(ny, nx, data, result);
        end = std::chrono::high_resolution_clock::now();
        fast_times[i] = std::chrono::duration<double>(end - start).count();
        
        std::cout << "  OpenMP: " << omp_times[i] << " sec, Fast: " << fast_times[i] << " sec\n";
    }

    // Print results table
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "PERFORMANCE SUMMARY\n";
    std::cout << std::string(80, '=') << "\n\n";

    // Table header
    std::cout << std::left << std::setw(10) << "Threads"
              << std::right << std::setw(12) << "Sequential"
              << std::setw(12) << "OpenMP"
              << std::setw(12) << "Fast"
              << std::setw(12) << "Speedup"
              << std::setw(12) << "Efficiency"
              << "\n";
    std::cout << std::string(80, '-') << "\n";

    // Print data
    for (int i = 0; i < num_tests; i++) {
        int threads = thread_counts[i];
        double speedup = seq_time / fast_times[i];
        double efficiency = (speedup / threads) * 100.0;
        
        std::cout << std::left << std::setw(10) << threads
                  << std::right << std::fixed << std::setprecision(6)
                  << std::setw(12) << seq_time
                  << std::setw(12) << omp_times[i]
                  << std::setw(12) << fast_times[i]
                  << std::setprecision(2)
                  << std::setw(11) << speedup << "x"
                  << std::setw(11) << efficiency << "%"
                  << "\n";
    }

    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << "Note: Speedup and Efficiency calculated relative to Sequential baseline\n";
    std::cout << "      using Fast (OpenMP+SIMD) implementation\n";
    std::cout << std::string(80, '=') << "\n\n";

    delete[] data;
    delete[] result;

    return 0;
}
