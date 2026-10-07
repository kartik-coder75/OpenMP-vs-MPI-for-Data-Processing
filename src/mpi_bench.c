#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_N 100000000

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long local_n = TOTAL_N / size;
    double *local_data = (double *)malloc(local_n * sizeof(double));
    if (!local_data) {
        fprintf(stderr, "Rank %d: Allocation failed!\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    for (long i = 0; i < local_n; i++) {
        local_data[i] = 1.0;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    double local_sum = 0.0;
    for (long i = 0; i < local_n; i++) {
        local_sum += (local_data[i] * 2.0 + 1.5);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double t_end = MPI_Wtime();

    if (rank == 0) {
        printf("[MPI - %d Processes] Result: %.2f | Time: %.4f seconds\n",
               size, global_sum, t_end - t_start);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}