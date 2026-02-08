import matplotlib.pyplot as plt
import numpy as np

# Actual experimental results
schedules = ['static', 'dynamic,4', 'guided']
t_max = [0.0590, 0.0810, 0.0600]
t_avg = [0.0383, 0.0533, 0.0489]
imbalance = [54.00, 51.93, 22.61]

# Create figure with 2 subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))

# 1. Execution Time Comparison
x = np.arange(len(schedules))
width = 0.35

bars1 = ax1.bar(x - width/2, t_max, width, label='T_max', color='coral')
bars2 = ax1.bar(x + width/2, t_avg, width, label='T_avg', color='skyblue')

ax1.set_xlabel('Scheduling Strategy', fontsize=12)
ax1.set_ylabel('Time (seconds)', fontsize=12)
ax1.set_title('Execution Time: T_max vs T_avg', fontsize=14, fontweight='bold')
ax1.set_xticks(x)
ax1.set_xticklabels(schedules)
ax1.legend(fontsize=11)
ax1.grid(True, alpha=0.3, axis='y')

# Add value labels on bars
for bars in [bars1, bars2]:
    for bar in bars:
        height = bar.get_height()
        ax1.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.2f}s', ha='center', va='bottom', fontsize=9)

# 2. Load Imbalance Comparison
bars = ax2.bar(schedules, imbalance, color=['red', 'orange', 'green'])
ax2.set_xlabel('Scheduling Strategy', fontsize=12)
ax2.set_ylabel('Imbalance (%)', fontsize=12)
ax2.set_title('Load Imbalance Comparison', fontsize=14, fontweight='bold')
ax2.grid(True, alpha=0.3, axis='y')

# Add value labels on bars
for bar in bars:
    height = bar.get_height()
    ax2.text(bar.get_x() + bar.get_width()/2., height,
            f'{height:.2f}%', ha='center', va='bottom', fontsize=10)

plt.tight_layout()
plt.savefig('scheduling_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("Scheduling comparison plot saved as 'scheduling_comparison.png'")

# Individual plots
# Imbalance plot
plt.figure(figsize=(10, 6))
colors = ['red', 'orange', 'green']
bars = plt.bar(schedules, imbalance, color=colors, edgecolor='black', linewidth=1.5)
plt.xlabel('Scheduling Strategy', fontsize=12)
plt.ylabel('Load Imbalance (%)', fontsize=12)
plt.title('Load Imbalance Across Different Scheduling Strategies', fontsize=14, fontweight='bold')
plt.grid(True, alpha=0.3, axis='y')

# Add value labels
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
            f'{height:.2f}%', ha='center', va='bottom', fontsize=11, fontweight='bold')

plt.tight_layout()
plt.savefig('imbalance_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

# Execution Time Comparison
plt.figure(figsize=(10, 6))
x = np.arange(len(schedules))
width = 0.35

bars1 = plt.bar(x - width/2, t_max, width, label='T_max', color='coral', edgecolor='black')
bars2 = plt.bar(x + width/2, t_avg, width, label='T_avg', color='skyblue', edgecolor='black')

plt.xlabel('Scheduling Strategy', fontsize=12)
plt.ylabel('Time (seconds)', fontsize=12)
plt.title('Thread Execution Time: Maximum vs Average', fontsize=14, fontweight='bold')
plt.xticks(x, schedules)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3, axis='y')

# Add value labels
for bars in [bars1, bars2]:
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                f'{height:.1f}s', ha='center', va='bottom', fontsize=9)

plt.tight_layout()
plt.savefig('time_comparison.png', dpi=300, bbox_inches='tight')
plt.show()

print("\nAll plots saved:")
print("- scheduling_comparison.png")
print("- imbalance_comparison.png")
print("- time_comparison.png")