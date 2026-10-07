# OpenMP vs. MPI for Data Processing

A comprehensive experimental evaluation comparing shared-memory parallelism (**OpenMP**) against distributed-memory message passing (**MPI**) on an identical data processing and reduction operation over a 100,000,000-element dataset.

---

## 1. Introduction & Objectives

High-performance computing relies on two primary paradigms for CPU parallelization:
- **OpenMP (Shared-Memory Model):** Operates on a unified address space where threads communicate implicitly through shared memory, coordinated by compiler pragmas and runtime thread pools.
- **MPI (Distributed-Memory Model):** Operates under a shared-nothing architecture where independent processes maintain isolated address spaces and communicate via explicit network or IPC messaging.

### Lab Evaluation Objective
1. Implement the exact same data transformation and reduction kernel in Sequential C, OpenMP, and MPI.
2. Ensure mathematical and numerical invariance across all executions.
3. Measure wall-clock execution time across varying processing units ($p \in \{1, 2, 4, 8\}$).
4. Quantify and evaluate **Speedup ($S_p$)** and **Parallel Efficiency ($E_p$)**.

---

## 2. Computational Kernel & Mathematical Formulation

The computational kernel performs a fused vector transformation and global summation reduction over a dataset of $N = 100,000,000$ double-precision floating-point numbers (~800 MB in memory):

$$\text{Result} = \sum_{i=0}^{N-1} \big(A[i] \times 2.0 + 1.5\big) \quad \text{where } A[i] = 1.0 \ \forall i$$

$$\text{Theoretical Invariant} = 100,000,000 \times (1.0 \times 2.0 + 1.5) = 350,000,000.00$$

Every run was verified against this exact value to confirm data race prevention and reduction correctness.

---

## 3. Repository Structure

```text
├── data/
│   └── benchmark_data.csv        # Tabulated execution times and metrics
├── graphs/
│   ├── execution_time_comparison.png
│   └── speedup_comparison.png
├── presentation/
│   └── pgc_lab_evaluation.pptx   # Final Lab Defense Slide Deck
├── report/
│   └── lab_evaluation_report.md  # Detailed technical evaluation report
├── results/
│   └── terminal_outputs.txt      # Raw console logs from WSL execution
├── src/
│   ├── baseline.c                # Single-threaded sequential implementation
│   ├── omp_bench.c               # OpenMP shared-memory implementation
│   └── mpi_bench.c               # MPI distributed-memory implementation
└── README.md
```
--- 

## 4. Step-by-Step Procedure & Execution

### Environment Prerequisites

- Linux / WSL2 Ubuntu environment
- GCC compiler with OpenMP support
- MPICH runtime and developer headers

```bash
sudo apt-get update
sudo apt-get install -y build-essential mpich libmpich-dev python3-pip
```

### Step 1: Compilation

All targets are compiled with aggressive optimization (`-O3`):

```bash
# Sequential Baseline
gcc -O3 src/baseline.c -o baseline

# OpenMP Binary
gcc -O3 -fopenmp src/omp_bench.c -o omp_bench

# MPI Binary
mpicc -O3 src/mpi_bench.c -o mpi_bench
```

### Step 2: Benchmarking Execution

Execute the binaries across varying core and thread counts:

```bash
# 1. Sequential Execution
./baseline

# 2. OpenMP Multi-threaded Runs
OMP_NUM_THREADS=2 ./omp_bench
OMP_NUM_THREADS=4 ./omp_bench
OMP_NUM_THREADS=8 ./omp_bench

# 3. MPI Multi-process Runs
mpirun -np 2 ./mpi_bench
mpirun -np 4 ./mpi_bench
mpirun -np 8 ./mpi_bench
```

---

## 5. Experimental Results & Performance Metrics

### Evaluation Formulas

$$S_p = \frac{T_1}{T_p}$$

$$E_p = \left(\frac{S_p}{p}\right) \times 100\%$$

Where:

- $T_1 = 0.0653\ s$ (sequential single-core wall time)
- $T_p$ = execution time using $p$ processing units (threads or processes)

### Results Table

| Paradigm   | Processing Units (p) | Execution Time (T<sub>p</sub>) | Speedup (S<sub>p</sub>) | Efficiency (E<sub>p</sub>) | Verification Invariant       |
|------------|:--------------------:|:------------------------------:|:-----------------------:|:--------------------------:|:----------------------------:|
| Sequential | 1                    | 0.0653 s                       | 1.00×                   | 100.0%                     | 350000000.00 (PASS)          |
| OpenMP     | 2                    | 0.0434 s                       | 1.50×                   | 75.2%                      | 350000000.00 (PASS)          |
| OpenMP     | 4                    | 0.0327 s                       | 2.00×                   | 49.9%                      | 350000000.00 (PASS)          |
| OpenMP     | 8                    | 0.0250 s                       | 2.61×                   | 32.7%                      | 350000000.00 (PASS)          |
| MPI        | 2                    | 0.0378 s                       | 1.73×                   | 86.4%                      | 350000000.00 (PASS)          |
| MPI        | 4                    | 0.0249 s                       | 2.62×                   | 65.6%                      | 350000000.00 (PASS)          |
| MPI        | 8                    | 0.0246 s                       | 2.65×                   | 33.2%                      | 350000000.00 (PASS)          |

---

## 6. In-Depth Comparative Analysis

### 1. Superior Early Scaling of MPI (p = 2, 4)

MPI outperformed OpenMP at moderate parallelism (1.73× vs 1.50× at p=2; 2.62× vs 2.00× at p=4). In MPI, each process allocates an isolated buffer of size N/p, providing dedicated cache spatial locality and avoiding cache-line invalidation over a shared heap.

### 2. OpenMP Shared-Memory Bus Saturation

OpenMP showed consistent speedup up to 8 threads (2.61×), but efficiency dropped from 75.2% to 32.7%. For a streaming array transformation, multiple threads concurrently accessing a shared memory space saturate the physical DDR bus bandwidth.

### 3. MPI Communication Saturation Plateau (p = 8)

From 4 to 8 processes, MPI execution time plateaued (0.0249 s → 0.0246 s). Once per-process computation dropped below 25 ms, inter-process communication overhead and tree barrier latency in `MPI_Reduce` counterbalanced additional compute parallelism.

---

## 7. Conclusions & Architectural Trade-offs

- **OpenMP** is preferable for shared-memory workstations where zero-copy, in-place processing is needed without duplicating address space overhead.
- **MPI** is essential for scaling across cluster nodes where physical memory is distributed, and delivers high cache efficiency when data fits well in per-process cache tiers.
- **Hybrid HPC Model (MPI + OpenMP)** is optimal for production clusters: MPI distributes workloads across physical cluster nodes, while OpenMP parallelizes loops across cores within each node.