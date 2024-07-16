#include <ralgo/lp/point_in_hexagon.h>
#include <ralgo/util/index_brute_force.h>

#include <ralgo/clinalg/solve.h>
#include <igris/dprint.h>

/**
	Решает задачу о нахождении точки внутри симлекса - выпуклой оболочке n+1 точки n мерного пространства.
*/
int point_in_simplex_d(
    double * A,
    int dim,
    double * target
)
{
	double matrix [(dim + 1) * (dim + 1)];
	double exttgt[dim + 1];
	double result[dim + 1];

	for (int pnt = 0; pnt < dim + 1; ++pnt)
	{
		for (int i = 0; i < dim; ++i)
		{
			*(matrix + i * (dim + 1) + pnt) = *(A + pnt * dim + i);
		}
	}

	for (int i = 0; i < dim + 1; ++i)
	{
		*(matrix + dim * (dim + 1) + i) = 1;
	}

	for (int i = 0; i < dim; ++i)
	{
		*(exttgt + i) = target[i];
	}
	exttgt[dim] = 1;

	linalg_square_solve_d(matrix, dim+1, exttgt, result);

	for (int i = 0; i < dim + 1; ++i)
	{
		if (result[i] > 1 || result[i] < 0)
			return 0;
	}

	return 1;
}


int point_in_hexagon_d(
    double * A,
    int dim,
    int points,
    double * target
)
{
	double simplex[dim*(dim+1)];
	int base_coords[dim+1];
	index_brute_force_init(base_coords, dim + 1);

	do
	{
		for (int ii=0;ii<dim+1;++ii) 
		{
			int point = base_coords[ii];

			for (int j = 0; j < dim; ++j) 
			{
				*(simplex + ii*dim + j) = *(A + dim*point + j);
			}
		}

		if (point_in_simplex_d(simplex, dim, target)) 
			return 1;
	}
	while (index_brute_force_next(base_coords, dim + 1, points) == 0);

	return 0;
}
