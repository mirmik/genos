#include "libm.h"

double round(double x)
{
	return signbit(x) ? ceil(x - 0.5) : floor(x + 0.5);
}
