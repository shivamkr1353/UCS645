#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double calculate_pi_parallel(long long steps, int num_threads) {
    double step = 1.0 / (double)steps;
    double sum = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(num_threads)
    {
        double x;
        double local_sum = 0.0;

        #pragma omp for
        for (long long i = 0; i < steps; i++) {
            x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    double end = omp_get_wtime();

    return end - start;
}

int main() {
    int max_cores = 17;
    long long base_steps = 100000000;

    long long strong_total_steps = 500000000;

    printf("STRONG SCALING\n");
    printf("%-10s %-12s %s\n", "Cores", "Time (s)", "Speedup");

    double t_serial_strong = calculate_pi_parallel(strong_total_steps, 1);
    printf("%-10s %-12.4f %.2fx\n", "1", t_serial_strong, 1.00);

    for (int n = 2; n <= max_cores; n++) {
        double p_time = calculate_pi_parallel(strong_total_steps, n);
        char cores_label[20];
        sprintf(cores_label, "%d", n);
        printf("%-10s %-12.4f %.2fx\n", cores_label, p_time, t_serial_strong / p_time);
    }

    printf("\nWEAK SCALING\n");
    printf("%-10s %-15s %-12s %s\n", "Cores", "Work", "Time (s)", "Efficiency");

    double t_serial_weak = calculate_pi_parallel(base_steps, 1);
    printf("%-10s %-15lld %-12.4f %s\n", "1", base_steps, t_serial_weak, "100%");

    for (int n = 2; n <= max_cores; n++) {
        long long current_work = base_steps * n;
        double p_time = calculate_pi_parallel(current_work, n);
        double efficiency = (t_serial_weak / p_time) * 100.0;

        char cores_label[20];
        sprintf(cores_label, "%d", n);
        printf("%-10s %-15lld %-12.4f %.2f%%\n", cores_label, current_work, p_time, efficiency);
    }

    return 0;
}