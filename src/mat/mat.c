#include <stddef.h>
#include <stdio.h>

#include "mat.h"


#define BLOCK_TRESHOLD 4

#define max(a,b)                \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a > _b ? _a : _b;          \
})                              \

#define min(a,b)                \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a < _b ? _a : _b;          \
})                              \

// New gcc/clang statement expression min/max macros from https://stackoverflow.com/a/58532788

// TODO fix this not generating new seeds
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
    for (uint32_t i = 0; i < result->rows; ++i)
    {
        for (uint32_t j = 0; j < result->cols; ++j)
        {
            float acc = 0;
            for (uint32_t k = 0; k < a->cols; ++k)
            {
                // result->data[(i * result->cols) + j] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];

                acc += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
            }
            result->data[(i*result->cols) + j] = acc;
        }
    }
    // Calculate result at ij ie. loop over i and j
    // for (uint32_t i = 0; i < result->rows; ++i)
    // {
    //     for (uint32_t j = 0; j < result->cols; ++j)
    //     {
    //         uint32_t r_ind = (i * result->cols) + j;
    //         for (uint32_t k = 0; k < a->cols; ++k)
    //         {
    //             result->data[r_ind] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
    //         }
    //     }
    // }
    
}

void mat_mult(const Mat *a, const Mat *b, Mat *result)
{
    // Init memory range to 0
    // memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));
    // Calculate result at ij ie. loop over i and j

    // TODO Can you save the calculated values -> write later like in mat_mult_slow
    for (uint32_t i = 0; i < result->rows; ++i)
    {
        for (uint32_t k = 0; k < a->cols; ++k)
        {
            for (uint32_t j = 0; j < result->cols; ++j)
            {
                result->data[(i*result->cols) + j] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
            }
        }
    }
}

// Tiled version of the normal iterative algorithm ie. mat_mult.
// https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm#Cache_behavior
// Seems to be faster even for small matrices and A LOT faster for large ones.
void mat_mult_tiled(const Mat *a, const Mat *b, Mat *result)
{
    // Tile size should be chosen st. a single tile matrix of size tile_size x tile_size
    // fits into cache -> tile_size ~= sqrt(cache_size) (or something like this).
    const uint32_t tile_size = 64;

    // memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));

    // Doesn't look like pre-calculating and storing these matters.
    // uint32_t I_max = result->rows/tile_size;
    // uint32_t J_max = result->cols/tile_size;
    // uint32_t K_max = a->cols/tile_size;

    // Looks like the loop order only matters for inner loop
    for (uint32_t I = 0; I < result->rows / tile_size; I += tile_size)
        {
        for (uint32_t J = 0; J < result->cols / tile_size; J += tile_size)
        {
            for (uint32_t K = 0; K < a->cols / tile_size; K += tile_size)
            {

                // uint32_t I_min = min(I + tile_size, result->rows);
                // uint32_t J_min = min(J + tile_size, result->cols);
                // uint32_t K_min = min(K + tile_size, a->cols);

                // Inner loop multiplies block/tile matrices, loop order matters a lot here
                for (uint32_t i = I; i < min(I + tile_size, result->rows); ++i)
                {
                    for (uint32_t k = K; k < min(K + tile_size, a->cols); ++k)
                    {
                        for (uint32_t j = J; j < min(J + tile_size, result->cols); ++j)
                        {
                            result->data[(i * result->cols) + j] +=
                                a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
                        }
                    }
                }
            }
        }
    }
}

// Recursive cache oblivious-matrix multiplication.
// NOTE Make sure memory allocation works here for the newly created block matrices.
// void mat_mult_block(const Mat *a, const Mat *b, Mat *result)
// {
//     // Find the max dimension of the multiplied matrices a,b.
//     // NOTE that a->cols = b->rows is required
//
//     // max_dim = max(n,m,p) when a is n x m and b is m x p.
//     uint32_t max_dim = a->rows;
//
//     // TODO better loop etc.?
//     if (a->cols > max_dim)
//     {
//         max_dim = a->cols;
//     }
//     if (b->cols > max_dim)
//     {
//         max_dim = b->cols;
//     }
//
//     // Handle recursion based on max_dim.
//
//     // If dimension is below a specified treshold, use the normal mat_mult algorithm.
//     if (max_dim < BLOCK_TRESHOLD)
//     {
//         // TODO Maybe make sure this unrolls/is optimized by the compiler etc.
//         mat_mult_block(a, b, result);
//     }
//     else if (max_dim == a->rows)
//     {
//         // Split A horizontally:
//         // A = (A1 A2)^T
//         // NOTE Make sure that data here is correct.
//         Mat a1 = {
//             .rows = a->rows / 2,
//             .cols = a->cols,
//             .data = a->data,
//         };
//         Mat a2 = {
//             .rows = a->rows - a->rows / 2,
//             .cols = a->cols,
//             .data = a->data + ((size_t)(a->rows / 2 * a->cols)),
//         };
//     }
//     else if (max_dim == b->cols)
//     {
//         // split B vertically
//     }
//     // Maybe just else here
//     else if (max_dim == a->cols)
//     {
//         // split A vertically and B horizontally
//     }
//
//     // Recursion
// }

// void mat_mult_block2(const Mat *a, const Mat *b, Mat *result)
// {
//     // Outer loop just multiplies matrices the "normal way", ie using mat_mult
//     //
//     // Init memory range to 0
//     memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));
//     // Calculate result at ij ie. loop over i and j
//     for (uint32_t i = 0; i < result->rows; ++i)
//     {
//         for (uint32_t k = 0; k < a->cols; ++k)
//         {
//             for (uint32_t j = 0; j < result->cols; ++j)
//             {
//                 result->data[(i * result->cols) + j] += a->data[(i * a->cols) + k] * b->data[(k * b->cols) + j];
//             }
//         }
//     }
// }

// NOTE:Assumes that matrix b is transposed.
// Calculates matrix multiplication on row vectors of a and b for potentially better cache usage.
void mat_mult_trans(const Mat *a, const Mat *b, Mat *result)
{
    // Init memory range to 0
    // TODO maybe do this outside of the function instead
    // memset(result->data, 0, result->rows * result->cols * sizeof(*result->data));

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
