#ifndef RALGO_MATH_GRADIENT_H
#define RALGO_MATH_GRADIENT_H

#include <igris/compiler.h>

typedef double (*graient_descent_function_t)(double *coords);

__BEGIN_DECLS

void convex_gradient_descent(int dim,
                             int points,
                             double alpha,
                             double epsilon,
                             double *table,  // size: dim * points
                             double *target, // size: dim
                             double *result  // size: dim
);

__END_DECLS

#endif
