// Matrix / linearalg implementation in C.
// Inspired by https://www.youtube.com/watch?v=zJ4y_JNBRKg

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "mat/mat.h"

#define MAT_SIZE 512

int main(void)
{
    // Malloc a contiguous piece of memory for the lib
    float *mat_data = malloc(sizeof(*mat_data) * MAT_SIZE * MAT_SIZE * 3);

    // Widened cast to ptrdiff_t
    Mat a = {.rows = 2, .cols = 2, .data = mat_data + (ptrdiff_t)(0 * MAT_SIZE * MAT_SIZE)};
    Mat b = {.rows = 2, .cols = 2, .data = mat_data + (ptrdiff_t)(1 * MAT_SIZE * MAT_SIZE)};
    Mat c = {.rows = 2, .cols = 2, .data = mat_data + (ptrdiff_t)(2 * MAT_SIZE * MAT_SIZE)};

    mat_print(a);
    mat_mult(a, b, c);
    mat_print(a);

    printf("sizeof Mat pointer: %zu\n", sizeof(Mat *));
    printf("sizeof Mat        : %zu\n", sizeof(Mat));

    free(mat_data);

    return 0;
}
