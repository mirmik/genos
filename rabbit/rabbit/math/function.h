#ifndef RABBIT_MATH_FUNCTION_H
#define RABBIT_MATH_FUNCTION_H

#include <rabbit/types.h>

namespace rabbit 
{
	class function
	{
	public:
		virtual int value(real parameter, real* result) = 0;
		virtual ~function() = default;
	};
}

#endif