#ifndef RABBIT_MATH_MINIMIZE_GOLD_H
#define RABBIT_MATH_MINIMIZE_GOLD_H

#include <rabbit/types.h>

#define GOLD 1.6180339887498948482

namespace rabbit
{
	// Поиск минимума функции методом золотого сечения.
	class minimize_gold
	{
		rabbit::function * f;
		real a;
		real b;

		real eps = rabbit::epsilon;
		int maxiter = 50;

	public:
		minimize_gold(
		    rabbit::function * func,
		    real a,
		    real b) : f(func), a(a), b(b) 
		{}

		int doit()
		{
			int sts;
			real x1, x2, z, f1, f2;

			z = (b - a) / GOLD;
			x1 = b - z;
			x2 = a + z;

			if ((sts = f->value(x1, &f1))) return sts;
			if ((sts = f->value(x2, &f2))) return sts;

			while (maxiter-- && fabs(x2-x1) > eps)
			{
				//nos::println(a, x1, x2, b, f1, f2);
				if ( f1 >= f2 )
				{
					a = x1;
					x1 = x2;
					f1 = f2;
					x2 = b - (x1 - a);
					if ((sts = f->value(x1, &f1))) return sts;
				}
				else
				{
					b = x2;
					x2 = x1;
					f2 = f1;
					x1 = a + (b - x2);
					if ((sts = f->value(x2, &f2))) return sts;
				}
			}

			return 0;
		}
	};
}

#endif