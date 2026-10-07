#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100000000

int main() {
    double *data = (double *)malloc(N * sizeof(double));
    if (!data) { 
        fprintf(stderr, "Memory allocation failed!\n"); 
        return 1; 
    }

    for (long i = 0; i < N; i++) {
        data[i] = 1.0;
    }

    clock_t start = clock();

    double total = 0.0;
    for (long i = 0; i < N; i++) {
        total += (data[i] * 2.0 + 1.5);
    }

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("[Sequential] Result: %.2f | Time: %.4f seconds\n", total, elapsed);

    free(data);
    return 0;
}