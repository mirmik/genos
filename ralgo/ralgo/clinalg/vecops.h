#ifndef RALGO_CLINALG_VECOPS_H
#define RALGO_CLINALG_VECOPS_H

#include <igris/compiler.h>
#include <igris/dprint.h>
#include <math.h>

__BEGIN_DECLS

static inline double
vecops_distance_d(const double *a, const double *b, int dim)
{
    double acc = 0;

    for (int i = 0; i < dim; ++i)
    {
        double diff = a[i] - b[i];
        acc += diff * diff;
    }

    return sqrt(acc);
}

static inline void
vecops_sub_d(const double *a, const double *b, int dim, double *c)
{
    for (int i = 0; i < dim; ++i)
    {
        c[i] = a[i] - b[i];
    }
}

static inline void
vecops_add_d(const double *a, const double *b, int dim, double *c)
{
    for (int i = 0; i < dim; ++i)
    {
        c[i] = a[i] + b[i];
    }
}

static inline double vecops_dot_d(const double *a, const double *b, int dim)
{
    double acc = 0;
    for (int i = 0; i < dim; ++i)
    {
        acc += a[i] * b[i];
    }
    return acc;
}

static inline void vecops_dprint_d(const char *info, const double *a, int dim)
{

    dpr(info);
    dpr(":(");

    for (int i = 0; i < dim - 1; ++i)
    {
        dpr(a[i]);
        dpr(",");
    }
    dpr(a[dim - 1]);
    dpr(")");
    dln();
}

__END_DECLS

#endif
