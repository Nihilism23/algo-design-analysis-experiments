import csv
import matplotlib.pyplot as plt
import numpy as np

# 读取 CSV 文件
sizes = []
bubble_time = []
merge_time = []
quick_time = []
bubble_cmp = []
merge_cmp = []
quick_cmp = []

with open('sort_results.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        sizes.append(int(row['Size']))
        bubble_time.append(float(row['BubbleTime(ms)']))
        merge_time.append(float(row['MergeTime(ms)']))
        quick_time.append(float(row['QuickTime(ms)']))
        bubble_cmp.append(int(row['BubbleComparisons']))
        merge_cmp.append(int(row['MergeComparisons']))
        quick_cmp.append(int(row['QuickComparisons']))

# 转换为 numpy 数组
n = np.array(sizes)

# 创建图表
plt.figure(figsize=(14, 10))

# 子图1：执行时间对比
plt.subplot(2, 2, 1)
plt.plot(sizes, bubble_time, 'ro-', label='Bubble Sort', linewidth=2, markersize=6)
plt.plot(sizes, merge_time, 'go-', label='Merge Sort', linewidth=2, markersize=6)
plt.plot(sizes, quick_time, 'bo-', label='Quick Sort', linewidth=2, markersize=6)
plt.xlabel('Input Size (n)', fontsize=12)
plt.ylabel('Execution Time (ms)', fontsize=12)
plt.title('Sort Algorithm Execution Time Comparison', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)

# 子图2：比较次数对比
plt.subplot(2, 2, 2)
plt.plot(sizes, bubble_cmp, 'r--', label='Bubble Comparisons', linewidth=2)
plt.plot(sizes, merge_cmp, 'g--', label='Merge Comparisons', linewidth=2)
plt.plot(sizes, quick_cmp, 'b--', label='Quick Comparisons', linewidth=2)
plt.xlabel('Input Size (n)', fontsize=12)
plt.ylabel('Comparison Count', fontsize=12)
plt.title('Sort Algorithm Comparison Counts', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)

# 子图3：理论复杂度对比
plt.subplot(2, 2, 3)
theo_bubble = n ** 2 / 500000  # O(n^2) scaled
theo_nlogn = n * np.log2(n) / 500  # O(n log n) scaled

plt.plot(sizes, theo_bubble, 'r:', label='O(n²) Theoretical', linewidth=3)
plt.plot(sizes, theo_nlogn, 'g:', label='O(n log n) Theoretical', linewidth=3)
plt.plot(sizes, bubble_time, 'ro-', label='Bubble Sort Actual', linewidth=2)
plt.plot(sizes, merge_time, 'go-', label='Merge Sort Actual', linewidth=2)
plt.xlabel('Input Size (n)', fontsize=12)
plt.ylabel('Time / Scaled Value', fontsize=12)
plt.title('Theoretical vs Actual Complexity', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)

# 子图4：对数坐标轴展示
plt.subplot(2, 2, 4)
plt.plot(sizes, bubble_time, 'ro-', label='Bubble Sort')
plt.plot(sizes, merge_time, 'go-', label='Merge Sort')
plt.plot(sizes, quick_time, 'bo-', label='Quick Sort')
plt.xscale('log')
plt.yscale('log')
plt.xlabel('Input Size (n) [log scale]', fontsize=12)
plt.ylabel('Execution Time (ms) [log scale]', fontsize=12)
plt.title('Execution Time (Log Scale)', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, linestyle='--', alpha=0.7)

plt.tight_layout(pad=3.0)

# 保存图表
plt.savefig('sort_algorithm_comparison.png', dpi=150, bbox_inches='tight')
print("图表已保存为 sort_algorithm_comparison.png")

# 显示图表
plt.show()