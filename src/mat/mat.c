#include <stddef.h>
#include <stdio.h>

#include "mat.h"


void mat_genf(Mat m, float fn(void))
{
    for (uint32_t i = 0; i < m.rows * m.cols; ++i)
    {
        m.data[i] = fn();
    }
}

void mat_mult(Mat res, const Mat a, const Mat b)
{
    // Calculate res_ij
    for (uint32_t i = 0; i < res.rows; ++i)
    {
        for (uint32_t j = 0; j < res.cols; ++j)
        {
            uint32_t r_ind = (i * res.cols) + j;
            for (uint32_t k = 0; k < a.cols; ++k)
            {
                res.data[r_ind] += a.data[(i * a.cols) + k] * b.data[(k * b.cols) + j];
            }
        }
    }
}

void mat_print(Mat m)
{
    for (uint32_t i = 0; i < m.rows; ++i)
    {
        for (uint32_t j = 0; j < m.cols; ++j)
        {
            printf(" %6.2f", m.data[(i * m.cols) + j]);
        }
        printf("\n");
    }
}
