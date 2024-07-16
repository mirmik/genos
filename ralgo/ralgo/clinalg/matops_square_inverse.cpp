#include <ralgo/clinalg/matops.h>
#include <ralgo/linalg/matops.h>
#include <ralgo/linalg/matrix_view.h>

int matops_square_inverse_f(float *data, int n, int stride, float *out)
{
    ralgo::matrix_view_ro<float> src(data, n, n, stride);
    ralgo::matrix_view_ro<float> dst(out, n, n, stride);

    return ralgo::matops::square_matrix_inverse(src, dst);
}

int matops_square_inverse_d(double *data, int n, int stride, double *out)
{
    ralgo::matrix_view_ro<double> src(data, n, n, stride);
    ralgo::matrix_view_ro<double> dst(out, n, n, stride);

    return ralgo::matops::square_matrix_inverse(src, dst);
}

void matops_multiply_frrr(float *a, int n, int k, int m, float *b, float *c)
{
    ralgo::matrix_view_ro<float> A(a, n, k);
    ralgo::matrix_view_ro<float> B(b, k, m);
    ralgo::matrix_view_ro<float> C(c, n, m);

    ralgo::matops::multiply(A, B, C);
}

void matops_multiply_dccc(double *a, int n, int k, int m, double *b, double *c)
{
    ralgo::matrix_view_co<double> A(a, n, k);
    ralgo::matrix_view_co<double> B(b, k, m);
    ralgo::matrix_view_co<double> C(c, n, m);

    ralgo::matops::multiply(A, B, C);
}
