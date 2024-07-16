#ifndef RABBIT_MATH_OPTIMIZE_H
#define RABBIT_MATH_OPTIMIZE_H

#include <rabbit/math/mulvar_function.h>

namespace rabbit 
{
	class optimization 
	{
	public:
		rabbit::mulvar_function * minimized_function;

		real * minbound;
		real * maxbound;
		real * tmpzone; // for internal use

		uint8_t dim;

		std::linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647> randgen;

	public:
		optimization(
			minimized_function_t minimized_function,
			real * minbound,
			real * maxbound,
			uint8_t dim,
			real * tmpzone
		);

		// Get a random point from region.
		void random_point(real * point);

		// Random minimal point finding.
		void random_minimum_finder(int n, real * point, real * value);

		void
	};
}

#endif