#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mat.h"


void mat_genf(Mat m, float fn(void))
{
    for (uint32_t i = 0; i < m.rows * m.cols; ++i)
    {
        m.data[i] = fn();
    }
}

void mat_trans(const Mat a, Mat b)
{
    // memset(b.data, 0, b.rows * b.cols * sizeof(*b.data));
    // Traverse a in row major order

    // printf("b.rows = %i, b.cols = %i", b.rows, b.cols);

    for (uint32_t i = 0; i < a.rows; ++i)
    {
        for (uint32_t j = 0; j < a.cols; ++j)
        {

            b.data[(j * a.rows) + i] = a.data[(i * a.cols) + j];
        }
    }

    // Swap the row and col dimensions for b
    b.rows = a.cols;
    b.cols = a.rows;
}

void mat_mult_slow(const Mat a, const Mat b, Mat result)
{
    // Calculate result at ij ie. loop over i and j
    for (uint32_t i = 0; i < result.rows; ++i)
    {
        for (uint32_t j = 0; j < result.cols; ++j)
        {
            uint32_t r_ind = (i * result.cols) + j;
            for (uint32_t k = 0; k < a.cols; ++k)
            {
                result.data[r_ind] += a.data[(i * a.cols) + k] * b.data[(k * b.cols) + j];
            }
        }
    }
}

void mat_mult(const Mat a, const Mat b, Mat result)
{
    // Init memory range to 0
    memset(result.data, 0, result.rows * result.cols * sizeof(*result.data));
    // Calculate result at ij ie. loop over i and j
    for (uint32_t i = 0; i < result.rows; ++i)
    {
        for (uint32_t k = 0; k < a.cols; ++k)
        {
            for (uint32_t j = 0; j < result.cols; ++j)
            {
                result.data[(i * result.cols) + j] += a.data[(i * a.cols) + k] * b.data[(k * b.cols) + j];
            }
        }
    }
}

// TODO Transpose matrix b for potentially better memory access.
// Not sure if worth, depends on the transpose cost I guess
// if b is transposed "during init" or outside of this func then
// should be gigafast
void mat_mult_trans(const Mat a, const Mat b, Mat result)
{
    // Init memory range to 0
    // TODO maybe do this outside of the function instead
    memset(result.data, 0, result.rows * result.cols * sizeof(*result.data));

    // Transpose b ie. cols into rows.
    Mat bT = {.cols = b.rows, .rows = b.cols};
    for (uint32_t i = 0; i < bT.cols * bT.rows; ++i)
    {
        // TODO
    }
    for (uint32_t i = 0; i < result.rows; ++i)
    {
        for (uint32_t k = 0; k < a.cols; ++k)
        {
            for (uint32_t j = 0; j < result.cols; ++j)
            {
                result.data[(i * result.cols) + j] += a.data[(i * a.cols) + k] * b.data[(k * b.cols) + j];
            }
        }
    }
}

void mat_print(Mat m)
{
    printf("m.rows = %i, m.cols = %i\n", m.rows, m.cols);
    for (uint32_t i = 0; i < m.rows; ++i)
    {
        for (uint32_t j = 0; j < m.cols; ++j)
        {
            printf(" %6.2f", m.data[(i * m.cols) + j]);
        }
        printf("\n");
    }
}
