#include "math.h"

double modf(double x, double *iptr)
{
	int64_t i = x;
	*iptr = i;
	return x - i;
}
