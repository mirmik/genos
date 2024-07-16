#ifndef RABBIT_POWELL_MINIMIZATION_H
#define RABBIT_POWELL_MINIMIZATION_H

namespace rabbit 
{
	class powell_minimization 
	{
		real * D; // dim * dim size matrix
		mulvar_function * func;

		real * curpoint;

		real * tmp1;
		real * tmp2;

		powell_minimization(
			mulvar_function * func, 
			real * start, 
			real * D, 
			real * tmp1, 
			real * tmp2
		) :
			D(D),
			func(func),
			curpoint(start),
			tmp1(tmp1),
			tmp2(tmp2)
		{
			for (int i = 0; i < dim; ++i) 
				for (int j = 0; j < dim; ++j)
					D[i*dim+j] = i == j ? 1 : 0;
		}

		real * dn(int n) 
		{
			return D + n * dim;
		}

		void step() 
		{
			int minvariant = -1;
			real curpoint_value = value;

			for (int i = 0; i < dim; ++i) 
			{
				rabbit::vector_add(curpoint, dn(i), tmp1, dim);
			}
		}
	};
}

#endif