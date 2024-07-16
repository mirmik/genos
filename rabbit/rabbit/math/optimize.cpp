#ifndef RABBIT_MATH_OPTIMIZE_H
#define RABBIT_MATH_OPTIMIZE_H

#include <rabbit/math/optimize.h>

optimization::optimization(
    minimized_function_t minimized_function,
    real * minbound,
    real * maxbound,
    uint8_t dim,
    real * tmpzone
)
	:
	minimized_function(minimized_function),
	minbound(minbound),
	maxbound(maxbound),
	dim(dim),
	tmpzone(tmpzone)
{
	real fseed = 0;
	for (int i = 0; i < dim; ++i)
	{
		fseed += maxbound[i];
		fseed += minbound[i];
	}

	randgen.seed(fseed * 16553);
}

void optimization::random_point(real * point)
{
	for (int i = 0; i < dim; ++i)
	{
		randgen::result_type randval = randgen();
		real koeff =
		    (double)(randval - randgen.min()) /
		    (double)(randgen.max() - randgen.min());

		point[i] = (1 - k) * minbound[i] + k * maxbound[i];
	}
}

void optimization::random_minimum_finder(int n, real * point, real * value)
{
	real minvalue = std::limits<real>::max();

	for (int i = 0; i < n; ++i)
	{
		random_point(point);

		real curval = minimized_function(point);
		if (curval < minvalue)
		{
			minvalue = curval;
			memcpy(tmpzone, point, sizeof(real) * dim);
		}
	}
}

#endif