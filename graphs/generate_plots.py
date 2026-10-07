import matplotlib.pyplot as plt

units = [1, 2, 4, 8]
omp_times = [0.0653, 0.0434, 0.0327, 0.0250]
mpi_times = [0.0653, 0.0378, 0.0249, 0.0246]

omp_speedup = [1.00, 1.50, 2.00, 2.61]
mpi_speedup = [1.00, 1.73, 2.62, 2.65]

plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in plt.style.available else 'default')

# Plot 1: Execution Time
plt.figure(figsize=(7, 4.5))
plt.plot(units, omp_times, marker='o', linewidth=2, label='OpenMP (Threads)', color='#0284c7')
plt.plot(units, mpi_times, marker='s', linewidth=2, label='MPI (Processes)', color='#10b981')
plt.title('Execution Time vs Units (100M Elements)', fontsize=13, fontweight='bold')
plt.xlabel('Threads / Processes (p)', fontsize=11)
plt.ylabel('Execution Time (seconds)', fontsize=11)
plt.xticks(units)
plt.legend()
plt.tight_layout()
plt.savefig('graphs/execution_time_comparison.png', dpi=300)
plt.close()

# Plot 2: Speedup
plt.figure(figsize=(7, 4.5))
plt.plot(units, omp_speedup, marker='o', linewidth=2, label='OpenMP Speedup', color='#0284c7')
plt.plot(units, mpi_speedup, marker='s', linewidth=2, label='MPI Speedup', color='#10b981')
plt.plot(units, units, '--', color='#94a3b8', label='Ideal Linear Speedup')
plt.title('Speedup Comparison (Sp)', fontsize=13, fontweight='bold')
plt.xlabel('Threads / Processes (p)', fontsize=11)
plt.ylabel('Speedup Factor', fontsize=11)
plt.xticks(units)
plt.legend()
plt.tight_layout()
plt.savefig('graphs/speedup_comparison.png', dpi=300)
plt.close()

print("Graphs generated successfully in graphs/ directory.")