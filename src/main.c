// Matrix / linearalg implementation in C.
// Inspired by https://www.youtube.com/watch?v=zJ4y_JNBRKg

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mat/mat.h"

#include "rng/rng.h"

#define MAT_SIZE 4
#define MAT_ROWS 4
#define MAT_COLS 3
// TODO MAT_COLS and MAT_ROWS etc. currently using square matrices only

// Time diff from 2 timespecs
// TODO Maybe make this better
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
    // NOTE The size here should be enough for all 3 matrices, careful when rows != cols
    float *mat_data = malloc(sizeof(*mat_data) * (MAT_ROWS * MAT_COLS * 2 + MAT_ROWS * MAT_ROWS));

    // Widened cast to ptrdiff_t
    Mat a = {
        .rows = MAT_ROWS,
        .cols = MAT_COLS,
        .data = mat_data + (ptrdiff_t)(0 * MAT_ROWS * MAT_COLS),
    };
    Mat b = {
        .rows = MAT_COLS,
        .cols = MAT_ROWS,
        .data = mat_data + (ptrdiff_t)(1 * MAT_ROWS * MAT_COLS),
    };
    // Mat c = {
    //     .rows = MAT_ROWS,
    //     .cols = MAT_ROWS,
    //     .data = mat_data + (ptrdiff_t)(2 * MAT_ROWS * MAT_COLS),
    // };

    // Fill matrices a and b with random values using rng_norm
    mat_genf(a, rng_norm);
    printf("a:\n");
    mat_print(a);
    // mat_genf(b, rng_norm);
    mat_trans(a, b);
    printf("aT:\n");
    mat_print(b);

    // struct timespec t_start;
    // struct timespec t_end;
    // struct timespec t_diff;
    //
    // clock_gettime(CLOCK_MONOTONIC, &t_start);
    // mat_mult(a, b, c);
    // clock_gettime(CLOCK_MONOTONIC, &t_end);
    //
    // // Get diff and convert
    // get_tdiff(t_start, t_end, &t_diff);
    // long long diff = (t_diff.tv_sec * 1000000000LL) + t_diff.tv_nsec;
    //
    // printf("mat_mult time: %0.3f s\n", (float)diff / 1e9);

    // printf("sizeof Mat pointer: %zu\n", sizeof(Mat *));
    // printf("sizeof Mat        : %zu\n", sizeof(Mat));

    free(mat_data);

    return 0;
}
