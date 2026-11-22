#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mat.h"


void mat_genf(Mat *m, float fn(void))
{
    for (uint32_t i = 0; i < m->rows * m->cols; ++i)
    {
        m->data[i] = fn();
    }
}

// Naive implementation without cache optimizations.
// Pass by pointer st. b rows/cols can be changed.
void mat_trans(const Mat *a, Mat *b)
{
    // Swap the row and col dimensions for b
    b->rows = a->cols;
    b->cols = a->rows;

    // Traverse a in row major order
    for (uint32_t i = 0; i < a->rows; ++i)
    {
        for (uint32_t j = 0; j < a->cols; ++j)
        {

            b->data[(j * a->rows) + i] = a->data[(i * a->cols) + j];
        }
    }
}

// void mat_trans2(const Mat *a, Mat *b)
// {
//     //
// }
//
// void mat_trans_co(const Mat *a, Mat *b)
// {
//     //
// }
//
// void mat_trans_inplace(Mat *a)
// {
//     //
// }

void mat_mult_slow(const Mat *a, const Mat *b, Mat *result)
{
    // Calculate result at ij ie. loop over i and j
    for (uint32_t i = 0; i < result->rows; ++i)
    {
        for (uint32_t j = 0; j < result->cols; ++j)
        {
            uint32_t r_ind = (i * result->cols) + j;
            for (uint32_t k = 0; k < a->cols; ++k)
            {
                result->data[r_ind] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
            }
        }
    }
}

void mat_mult(const Mat *a, const Mat *b, Mat *result)
{
    // Init memory range to 0
    memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));
    // Calculate result at ij ie. loop over i and j
    for (uint32_t i = 0; i < result->rows; ++i)
    {
        for (uint32_t k = 0; k < a->cols; ++k)
        {
            for (uint32_t j = 0; j < result->cols; ++j)
            {
                result->data[(i * result->cols) + j] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
            }
        }
    }
}


// NOTE:Assumes that matrix b is transposed.
// Calculates matrix multiplication on row vectors of a and b for potentially better cache usage.
void mat_mult_trans(const Mat *a, const Mat *b, Mat *result)
{
    // Init memory range to 0
    // TODO maybe do this outside of the function instead
    memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));

    for (uint32_t i = 0; i < result->rows; ++i)
    {
        for (uint32_t k = 0; k < a->cols; ++k)
        {
            for (uint32_t j = 0; j < result->cols; ++j)
            {
                // TODO Check that b is correct here
                result->data[(i * result->cols) + j] += a->data[(i * a->cols) + k] * b->data[(j * b->cols) + k];
            }
        }
    }
}

void mat_print(Mat *m)
{
    printf("m.rows = %i, m.cols = %i\n", m->rows, m->cols);
    for (uint32_t i = 0; i < m->rows; ++i)
    {
        for (uint32_t j = 0; j < m->cols; ++j)
        {
            printf(" %6.2f", m->data[(i * m->cols) + j]);
        }
        printf("\n");
    }
}
