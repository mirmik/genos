#include <ralgo/lp/gradient.h>
#include <ralgo/clinalg/vecops.h>
#include <ralgo/clinalg/matops.h>

#include <unistd.h>
#include <string.h>
#include <igris/dprint.h>

/*void convex_gradient_descent(
	int dim,
	int points,
	double alpha,
	double epsilon,
	double * table,              // size: dim * points
	double * target,             // size: dim
	double * result              // size: dim
)
{
	double diff0[dim];
	double diff1[dim];
	double current[dim];
	double lastdist, distance;

	memset(result, 0, dim * points * sizeof(double));
	
	lastdist = 0;
	distance = 1000000000;
	while (distance > epsilon || lastdist == distance) 
	{
		lastdist = distance;

		for (int i = 0; i < points; ++i) 
		{
			matops_multiply_dccc(table, dim, points, 1, result, current);

			vecops_sub_d(table + i*dim, current, dim, diff0);
			vecops_sub_d(target,        current, dim, diff1);

			double koeff = vecops_dot_d(diff0, diff1, dim);
			result[i] += koeff * alpha; 

			if (result[i] > 1) 
				result[i] = 1;

			if (result[i] < 0) 
				result[i] = 0;
		}

		distance = vecops_distance_d(current, target, dim);
	}
}*/
