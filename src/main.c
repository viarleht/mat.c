// Matrix / linearalg implementation in C.
// Inspired by https://www.youtube.com/watch?v=zJ4y_JNBRKg

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mat/mat.h"

#include "rng/rng.h"

#define MAT_SIZE 512

// Time diff from 2 timespecs
void get_tdiff(struct timespec start, struct timespec end, struct timespec *diff)
{
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        diff->tv_sec = end.tv_sec - start.tv_sec - 1;
        diff->tv_nsec = 1000000000L + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        diff->tv_sec = end.tv_sec - start.tv_sec;
        diff->tv_nsec = end.tv_nsec - start.tv_nsec;
    }
}

int main(void)
{
    // Malloc a contiguous piece of memory for the lib
    float *mat_data = malloc(sizeof(*mat_data) * MAT_SIZE * MAT_SIZE * 3);

    // Widened cast to ptrdiff_t
    Mat a = {
        .rows = MAT_SIZE,
        .cols = MAT_SIZE,
        .data = mat_data + (ptrdiff_t)(0 * MAT_SIZE * MAT_SIZE),
    };
    Mat b = {
        .rows = MAT_SIZE,
        .cols = MAT_SIZE,
        .data = mat_data + (ptrdiff_t)(1 * MAT_SIZE * MAT_SIZE),
    };
    Mat c = {
        .rows = MAT_SIZE,
        .cols = MAT_SIZE,
        .data = mat_data + (ptrdiff_t)(2 * MAT_SIZE * MAT_SIZE),
    };

    // Fill matrices a and b with random values using rng_norm
    mat_genf(a, rng_norm);
    mat_genf(b, rng_norm);

    struct timespec t_start;
    struct timespec t_end;
    struct timespec t_diff;

    clock_gettime(CLOCK_MONOTONIC, &t_start);
    mat_mult(a, b, c);
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    // Get diff and convert
    get_tdiff(t_start, t_end, &t_diff);
    long long diff = (t_diff.tv_sec * 1000000000LL) + t_diff.tv_nsec;

    printf("mat_mult time: %0.3f s\n", (float)diff / 1e9);

    // printf("sizeof Mat pointer: %zu\n", sizeof(Mat *));
    // printf("sizeof Mat        : %zu\n", sizeof(Mat));

    free(mat_data);

    return 0;
}
