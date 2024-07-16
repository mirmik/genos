#ifndef RABBIT_MATH_MULTI_VARIABLE_FUNCTION_H
#define RABBIT_MATH_MULTI_VARIABLE_FUNCTION_H

#include <rabbit/types.h>

namespace rabbit 
{
	mulvar_function_type = real (*) (real *);

	class mulvar_function 
	{
	private:
		real (*func) (real *);
	
	public:
		mulvar_function(mulvar_function_type foo) : func(foo) {}
		real operator()(real * pnt)	{ return func(pnt); }

	public:
		virtual bool has_gradient() { return false; }
		virtual bool has_hessian() { return false; }
	};
}

#endif