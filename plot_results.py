import matplotlib.pyplot as plt
import numpy as np

# Sort algorithm data from C program output
sort_sizes = [100, 500, 1000, 2000, 5000, 10000, 20000]

# Actual time data from test results (ms)
bubble_time = [0.00, 0.00, 1.00, 4.00, 25.00, 107.00, 564.00]  # O(n^2)
merge_time = [0.00, 0.00, 0.00, 1.00, 0.00, 1.00, 3.00]  # O(n log n)
quick_time = [0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 1.00]  # O(n log n)

# Actual comparison count data from test results
bubble_cmp = [4950, 124750, 499500, 1999000, 12497500, 49995000, 199990000]
merge_cmp = [543, 3865, 8666, 19409, 55233, 120497, 260971]
quick_cmp = [649, 4496, 11406, 27754, 72191, 152211, 343775]

# Theoretical complexity curves
n = np.array(sort_sizes)
theo_bubble = n ** 2 / 1000000  # O(n^2) scaled
theo_nlogn = n * np.log2(n) / 1000  # O(n log n) scaled

plt.figure(figsize=(12, 8))

plt.subplot(2, 2, 1)
plt.plot(sort_sizes, bubble_time, 'ro-', label='Bubble Sort')
plt.plot(sort_sizes, merge_time, 'go-', label='Merge Sort')
plt.plot(sort_sizes, quick_time, 'bo-', label='Quick Sort')
plt.xlabel('Input Size (n)')
plt.ylabel('Time (ms)')
plt.title('Sort Algorithm Execution Time')
plt.legend()
plt.grid(True)

plt.subplot(2, 2, 2)
plt.plot(sort_sizes, bubble_cmp, 'r--', label='Bubble Comparisons')
plt.plot(sort_sizes, merge_cmp, 'g--', label='Merge Comparisons')
plt.plot(sort_sizes, quick_cmp, 'b--', label='Quick Comparisons')
plt.xlabel('Input Size (n)')
plt.ylabel('Comparison Count')
plt.title('Sort Algorithm Comparison Counts')
plt.legend()
plt.grid(True)

plt.subplot(2, 2, 3)
plt.plot(sort_sizes, theo_bubble, 'r:', label='O(n^2)')
plt.plot(sort_sizes, theo_nlogn, 'g:', label='O(n log n)')
plt.plot(sort_sizes, bubble_time, 'ro-', label='Bubble Actual')
plt.plot(sort_sizes, merge_time, 'go-', label='Merge Actual')
plt.xlabel('Input Size (n)')
plt.ylabel('Time (scaled)')
plt.title('Theoretical vs Actual Complexity')
plt.legend()
plt.grid(True)

# Knapsack data from actual test results
knapsack_items = [50, 100, 200, 500, 1000]
capacity_1000_dp = [0.00, 0.00, 1.00, 3.00, 5.00]
capacity_1000_greedy = [0.00, 0.00, 0.00, 0.00, 0.00]
capacity_5000_dp = [1.00, 2.00, 5.00, 12.00, 25.00]
capacity_5000_greedy = [0.00, 0.00, 0.00, 0.00, 0.00]
capacity_10000_dp = [3.00, 7.00, 11.00, 29.00, 59.00]
capacity_10000_greedy = [0.00, 0.00, 0.00, 0.00, 0.00]

plt.subplot(2, 2, 4)
plt.plot(knapsack_items, capacity_1000_dp, 'ro-', label='DP (C=1000)')
plt.plot(knapsack_items, capacity_5000_dp, 'go-', label='DP (C=5000)')
plt.plot(knapsack_items, capacity_10000_dp, 'bo-', label='DP (C=10000)')
plt.plot(knapsack_items, capacity_1000_greedy, 'r--', label='Greedy (C=1000)')
plt.plot(knapsack_items, capacity_5000_greedy, 'g--', label='Greedy (C=5000)')
plt.plot(knapsack_items, capacity_10000_greedy, 'b--', label='Greedy (C=10000)')
plt.xlabel('Item Count (n)')
plt.ylabel('Time (ms)')
plt.title('Knapsack Algorithm Execution Time')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('algorithm_performance.png', dpi=150, bbox_inches='tight')
print("图表已保存为 algorithm_performance.png")

# Show the plot
plt.show()