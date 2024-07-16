#ifndef RALGO_BASE_SOLVERS_FINDER_H
#define RALGO_BASE_SOLVERS_FINDER_H

#include <igris/compiler.h>

__BEGIN_DECLS

/**
    Решает задачу о нахождении точки внутри симлекса - выпуклой оболочке n+1
   точки n мерного пространства.
*/
int point_in_simplex_d(double *A, int dim, double *target);

/**
    A[dim][points] - заданной набором точек.
    dim - мерность пространства,
    points - количество точек, задающих оболочку.
    target[dim] - координаты искомой точки.
    return - 1, если решение найдено.
*/
int point_in_hexagon_d(double *A, int dim, int points, double *target);

__END_DECLS

#endif
