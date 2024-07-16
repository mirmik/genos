#ifndef RALGO_CLINALG_MATOPS_H
#define RALGO_CLINALG_MATOPS_H

#include <igris/compiler.h>

/**
    r - row order
    c - column order
*/

__BEGIN_DECLS

/// Инверсия квадратной матрицы
int matops_square_inverse_f(float *data, int n, int stride, float *out);
int matops_square_inverse_d(double *data, int n, int stride, double *out);

void matops_multiply_frrr(float *a, int n, int k, int m, float *b, float *c);
void matops_multiply_dccc(double *a, int n, int k, int m, double *b, double *c);

__END_DECLS

#endif
