#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000

int main() {
    double *data = (double *)malloc(N * sizeof(double));
    if (!data) { 
        fprintf(stderr, "Memory allocation failed!\n"); 
        return 1; 
    }

    #pragma omp parallel for schedule(static)
    for (long i = 0; i < N; i++) {
        data[i] = 1.0;
    }

    double t_start = omp_get_wtime();

    double total = 0.0;
    #pragma omp parallel for reduction(+:total) schedule(static)
    for (long i = 0; i < N; i++) {
        total += (data[i] * 2.0 + 1.5);
    }

    double t_end = omp_get_wtime();

    printf("[OpenMP - %d Threads] Result: %.2f | Time: %.4f seconds\n",
           omp_get_max_threads(), total, t_end - t_start);

    free(data);
    return 0;
}