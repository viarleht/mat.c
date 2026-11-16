#ifndef _MAT_H
// TODO check this def
#define _MAT_H

#include <stdint.h>

typedef struct
{
    // Memory layout:
    // rows + cols + data* = 4 + 4 + 8 = 16
    uint32_t rows, cols;

    // Matrix elements mapped to 1d array st.
    //
    // | a11 a12 a13 .. a1cols |
    // | a21 ..                |
    // | ..
    // | arows1 ..    arowscols|
    //
    // maps to {a11, a12, ... , a1cols, a21 ... , arowscols} etc.
    // Access mat element aij from data using
    // m.data[i * m.cols + j]
    float *data;
} Mat;

// Generate matrix elements based on a function fn
void mat_genf(Mat m, float fn(void));

void mat_print(Mat m);

void mat_mult(Mat res, Mat a, Mat b);

#endif // _MAT_H
