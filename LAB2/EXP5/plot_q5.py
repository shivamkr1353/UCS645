import matplotlib.pyplot as plt
import numpy as np

# Experimental results with 1-17 threads
threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
critical_times = [0.160, 0.312, 0.277, 0.333, 0.457, 0.535, 0.587, 0.651, 0.671, 0.674, 0.684, 0.692, 0.714, 0.706, 0.709, 0.708, 0.718]
reduction_times = [0.021, 0.014, 0.014, 0.007, 0.007, 0.009, 0.007, 0.003, 0.006, 0.009, 0.002, 0.007, 0.002, 0.005, 0.007, 0.003, 0.005]
overhead_factors = [t_c / t_r for t_c, t_r in zip(critical_times, reduction_times)]

# Create figure with 2x2 subplots
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 10))

# 1. Execution Time vs Threads
ax1.plot(threads, critical_times, marker='o', linewidth=2, markersize=8, color='red', label='Critical Section')
ax1.plot(threads, reduction_times, marker='s', linewidth=2, markersize=8, color='green', label='Reduction')
ax1.set_xlabel('Number of Threads', fontsize=11)
ax1.set_ylabel('Execution Time (seconds)', fontsize=11)
ax1.set_title('Execution Time: Critical Section vs Reduction', fontsize=12, fontweight='bold')
ax1.grid(True, alpha=0.3)
ax1.legend()
ax1.set_xticks(threads)

# 2. Overhead Factor vs Threads
ax2.plot(threads, overhead_factors, marker='^', linewidth=2, markersize=8, color='purple')
ax2.set_xlabel('Number of Threads', fontsize=11)
ax2.set_ylabel('Overhead Factor (times slower)', fontsize=11)
ax2.set_title('Critical Section Overhead vs Reduction', fontsize=12, fontweight='bold')
ax2.grid(True, alpha=0.3)
ax2.set_xticks(threads)

# 3. Log Scale Comparison
ax3.semilogy(threads, critical_times, marker='o', linewidth=2, markersize=8, color='red', label='Critical Section')
ax3.semilogy(threads, reduction_times, marker='s', linewidth=2, markersize=8, color='green', label='Reduction')
ax3.set_xlabel('Number of Threads', fontsize=11)
ax3.set_ylabel('Execution Time (seconds, log scale)', fontsize=11)
ax3.set_title('Execution Time (Log Scale)', fontsize=12, fontweight='bold')
ax3.grid(True, alpha=0.3)
ax3.legend()
ax3.set_xticks(threads)

# 4. Speedup of Reduction over Critical Section
speedups = [t_c / t_r for t_c, t_r in zip(critical_times, reduction_times)]
ax4.plot(threads, speedups, marker='d', linewidth=2, markersize=8, color='blue')
ax4.set_xlabel('Number of Threads', fontsize=11)
ax4.set_ylabel('Speedup Factor', fontsize=11)
ax4.set_title('Reduction Speedup over Critical Section', fontsize=12, fontweight='bold')
ax4.grid(True, alpha=0.3)
ax4.set_xticks(threads)

plt.tight_layout()
plt.savefig('sync_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("Synchronization comparison plot saved as 'sync_comparison.png'")

# Individual plot - Execution Time Comparison
plt.figure(figsize=(12, 6))
x = np.arange(len(threads))
width = 0.35

bars1 = plt.bar(x - width/2, critical_times, width, label='Critical Section', color='red', edgecolor='black')
bars2 = plt.bar(x + width/2, reduction_times, width, label='Reduction', color='green', edgecolor='black')

plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Execution Time (seconds)', fontsize=12)
plt.title('Synchronization Overhead: Critical Section vs Reduction', fontsize=14, fontweight='bold')
plt.xticks(x, threads)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('execution_time_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

# Overhead Factor Plot
plt.figure(figsize=(10, 6))
plt.plot(threads, overhead_factors, marker='o', linewidth=2, markersize=8, color='purple')
plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Overhead Factor (Critical / Reduction)', fontsize=12)
plt.title('Critical Section Overhead Factor vs Thread Count', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3)
plt.xticks(threads)

for i, (t, oh) in enumerate(zip(threads, overhead_factors)):
    if i % 2 == 0:  # Show labels for every other point to avoid clutter
        plt.text(t, oh + 5, f'{oh:.1f}x', ha='center', fontsize=9)

plt.tight_layout()
plt.savefig('overhead_factor.png', dpi=300, bbox_inches='tight')
plt.show()

print("\nAll plots saved:")
print("- sync_comparison.png")
print("- execution_time_comparison.png")
print("- overhead_factor.png")
