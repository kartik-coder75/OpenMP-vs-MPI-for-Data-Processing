# Parallel and GPU Computing Mini Project Report

## Experiment: OpenMP vs. MPI for Large-Scale Data Processing

### 1. Abstract
This experiment compares shared-memory (OpenMP) and distributed-memory (MPI) programming paradigms for high-throughput stream processing on a 100-million double-precision dataset. Performance was evaluated on speedup, scaling efficiency, and resource bottlenecks.

### 2. Experimental Setup
- **Operating Environment:** WSL2 Ubuntu (x86_64)
- **Compiler:** GCC 13.x with `-O3 -fopenmp`
- **MPI Runtime:** MPICH 4.3.2 (`ch4:ucx` device)
- **Dataset Size:** $N = 100,000,000$ double precision floats (~800 MB footprint)
- **Kernel:** Array transformation and reduction $\sum (A[i] \times 2.0 + 1.5)$

### 3. Key Findings
1. **Mathematical Consistency:** All 7 executions produced an identical invariant sum of `350000000.00`.
2. **MPI Cache Advantage:** MPI outperformed OpenMP at 2 cores ($1.73\times$ vs $1.50\times$) and 4 cores ($2.62\times$ vs $2.00\times$) because independent process address spaces maximized L1/L2 data locality and eliminated shared-heap cache line contention.
3. **Communication Plateau:** MPI leveled off between 4 and 8 processes ($0.0249\text{ s} \rightarrow 0.0246\text{ s}$), where inter-process communication (IPC) and `MPI_Reduce` tree barrier latencies offset the computational benefit of smaller array partitions.