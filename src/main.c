// Matrix / linearalg implementation in C.
// Inspired by https://www.youtube.com/watch?v=zJ4y_JNBRKg

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mat/mat.h"

#include "rng/rng.h"

#define MAT_SIZE 4
#define MAT_ROWS 4
#define MAT_COLS 3

// Large matrices
// #define MAT_SIZE 1024
// #define MAT_ROWS 1024
// #define MAT_COLS 960

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
    // NOTE The size here should be enough for all used matrices, careful when rows != cols

    // NOTE Assume that MAT_SIZE = max(MAT_COLS,MAT_ROWS), maybe use a macro for this later.
    //
    // float *mat_data = malloc(sizeof(*mat_data) * (MAT_ROWS * MAT_COLS * 2 + MAT_ROWS * MAT_ROWS));
    float *mat_data = malloc(sizeof(*mat_data) * MAT_SIZE * MAT_SIZE * 5);

    // Widened cast to ptrdiff_t, check later
    Mat a = {
        .rows = MAT_ROWS,
        .cols = MAT_COLS,
        // Sizes here should be the max of cols/rows st. no memory overlaps.
        // Additionally could define MAT_SIZE to be this max so you can just use that
        .data = mat_data + (ptrdiff_t)(0 * MAT_SIZE * MAT_SIZE),
    };
    Mat b = {
        .rows = MAT_COLS,
        .cols = MAT_ROWS,
        .data = mat_data + (ptrdiff_t)(1 * MAT_SIZE * MAT_SIZE),
    };
    Mat c = {
        .rows = MAT_ROWS,
        .cols = MAT_ROWS,
        .data = mat_data + (ptrdiff_t)(2 * MAT_SIZE * MAT_SIZE),
    };
    // Mat d = {
    //     .rows = MAT_ROWS,
    //     .cols = MAT_ROWS,
    //     .data = mat_data + (ptrdiff_t)(3 * MAT_SIZE * MAT_SIZE),
    // };

    // Fill matrices a and b with random values using rng_norm
    mat_genf(&a, rng_norm);
    // printf("a:\n");
    // mat_print(&a);

    mat_genf(&b, rng_norm);
    // mat_print(&b);
    // mat_print(&c);

    struct timespec t_start;
    struct timespec t_end;
    struct timespec t_diff;

    // Time start
    clock_gettime(CLOCK_MONOTONIC, &t_start);


    // Operation
    mat_mult_tiled(&a, &b, &c);
    // mat_mult(&a, &b, &c);
    // mat_mult_slow(&a, &b, &c);
    // mat_trans(&b, &c);
    // mat_mult_trans(&a, &c, &d);

    // Time end
    clock_gettime(CLOCK_MONOTONIC, &t_end);

    // mat_print(&c);

    // Get diff and convert
    get_tdiff(t_start, t_end, &t_diff);
    long long diff = (t_diff.tv_sec * 1000000000LL) + t_diff.tv_nsec;

    printf("Operation time: %0.10f s\n", (float)diff / 1e9);

    // printf("sizeof Mat pointer: %zu\n", sizeof(Mat *));
    // printf("sizeof Mat        : %zu\n", sizeof(Mat));

    // printf("aT:\n");
    // mat_print(&b);

    // Free memory just in case.
    free(mat_data);

    return 0;
}
