#ifndef MAT_H
#define MAT_H

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

    // TODO Check if this should be a pointer or not. Probably yes
    float *data;
} Mat;


// --- Constructor like ---
//
// Generate matrix elements based on a function fn(void) -> float
void mat_genf(Mat *m, float fn(void));


// --- Matrix Transpose ---
//
// Transposes matrix a and stores the result into matrix b.
void mat_trans(const Mat *a, Mat *b);
void mat_trans2(const Mat *a, Mat *b);
// Cache-oblivious matrix transpose.
void mat_trans_co(const Mat *a, Mat *b);
// Transposes matrix a in place.
void mat_trans_inplace(Mat *a);


// --- Matrix Multiplication ---
//
// NOTE Multiplication assumes that the result is a zero matrix!!
// Change this later if too inconvenient.
//
// Naive matrix multiplication with bad memory access pattern
void mat_mult_slow(const Mat *a, const Mat *b, Mat *result);
// Improved memory access pattern
void mat_mult(const Mat *a, const Mat *b, Mat *result);
// Transposed multiplication.
void mat_mult_trans(const Mat *a, const Mat *b, Mat *result);
void mat_mult_tiled(const Mat *a, const Mat *b, Mat *result);
// Cache oblivious matrix multiplication using matrix blocking.
void mat_mult_block(const Mat *a, const Mat *b, Mat *result);
void mat_mult_block2(const Mat *a, const Mat *b, Mat *result);


// --- Utility ---
//
void mat_print(Mat *m);

#endif // MAT_H
