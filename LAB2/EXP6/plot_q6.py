import matplotlib.pyplot as plt
import numpy as np

# Results from experiment with 1-17 threads
threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17]
false_sharing_times = [0.270, 0.437, 0.658, 0.894, 1.152, 1.304, 0.950, 1.484, 1.553, 1.827, 1.873, 1.839, 1.852, 1.973, 1.874, 1.942, 2.178]
padded_times = [0.277, 0.271, 0.286, 0.298, 0.354, 0.340, 0.333, 0.360, 0.399, 0.474, 0.464, 0.506, 0.526, 0.560, 0.595, 0.632, 0.638]
speedups = [fs/p for fs, p in zip(false_sharing_times, padded_times)]

# Create comprehensive visualization
fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 10))

# 1. Execution Time Comparison
ax1.plot(threads, false_sharing_times, marker='o', linewidth=2, markersize=8, color='red', label='False Sharing')
ax1.plot(threads, padded_times, marker='s', linewidth=2, markersize=8, color='green', label='Padded (Fixed)')
ax1.set_xlabel('Number of Threads', fontsize=11)
ax1.set_ylabel('Execution Time (seconds)', fontsize=11)
ax1.set_title('False Sharing vs Padded: Execution Time', fontsize=12, fontweight='bold')
ax1.grid(True, alpha=0.3)
ax1.legend()
ax1.set_xticks(threads)

# 2. Speedup from Padding
ax2.plot(threads, speedups, marker='^', linewidth=2, markersize=8, color='purple')
ax2.axhline(y=1, color='red', linestyle='--', linewidth=1, label='No improvement')
ax2.set_xlabel('Number of Threads', fontsize=11)
ax2.set_ylabel('Speedup Factor', fontsize=11)
ax2.set_title('Padding Speedup over False Sharing', fontsize=12, fontweight='bold')
ax2.grid(True, alpha=0.3)
ax2.legend()
ax2.set_xticks(threads)

# 3. Performance Gap (Time Difference)
time_diff = [fs - p for fs, p in zip(false_sharing_times, padded_times)]
ax3.bar(threads, time_diff, color='orange', edgecolor='black', alpha=0.7)
ax3.set_xlabel('Number of Threads', fontsize=11)
ax3.set_ylabel('Time Difference (seconds)', fontsize=11)
ax3.set_title('Performance Gap: False Sharing - Padded', fontsize=12, fontweight='bold')
ax3.grid(True, alpha=0.3, axis='y')
ax3.set_xticks(threads)

# 4. Overhead Percentage
overhead_pct = [(fs - p) / p * 100 for fs, p in zip(false_sharing_times, padded_times)]
ax4.plot(threads, overhead_pct, marker='d', linewidth=2, markersize=8, color='brown')
ax4.set_xlabel('Number of Threads', fontsize=11)
ax4.set_ylabel('False Sharing Overhead (%)', fontsize=11)
ax4.set_title('False Sharing Overhead vs Thread Count', fontsize=12, fontweight='bold')
ax4.grid(True, alpha=0.3)
ax4.set_xticks(threads)

plt.tight_layout()
plt.savefig('false_sharing_analysis.png', dpi=300, bbox_inches='tight')
plt.show()

print("False sharing analysis plot saved as 'false_sharing_analysis.png'")

# Individual plot - Side by side comparison
plt.figure(figsize=(12, 6))
x = np.arange(len(threads))
width = 0.35

bars1 = plt.bar(x - width/2, false_sharing_times, width, label='False Sharing', color='red', edgecolor='black')
bars2 = plt.bar(x + width/2, padded_times, width, label='Padded', color='green', edgecolor='black')

plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Execution Time (seconds)', fontsize=12)
plt.title('False Sharing vs Padded Memory Access', fontsize=14, fontweight='bold')
plt.xticks(x, threads)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3, axis='y')

plt.tight_layout()
plt.savefig('time_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

# Speedup plot
plt.figure(figsize=(10, 6))
plt.plot(threads, speedups, marker='o', linewidth=2, markersize=8, color='blue')
plt.axhline(y=1, color='red', linestyle='--', linewidth=1, alpha=0.5, label='No improvement')
plt.xlabel('Number of Threads', fontsize=12)
plt.ylabel('Speedup Factor (False Sharing / Padded)', fontsize=12)
plt.title('Performance Improvement with Padding', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3)
plt.xticks(threads)
plt.legend(fontsize=11)

for i, (t, s) in enumerate(zip(threads, speedups)):
    if i % 2 == 0:  # Show labels for every other point
        plt.text(t, s + 0.1, f'{s:.2f}x', ha='center', fontsize=9)

plt.tight_layout()
plt.savefig('speedup_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("\nAll plots saved:")
print("- false_sharing_analysis.png")
print("- time_comparison.png")
print("- speedup_comparison.png")
