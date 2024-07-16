#ifndef RABBIT_MATH_MINBRACKET_H
#define RABBIT_MATH_MINBRACKET_H

#include <rabbit/math/function.h>

#define GOLD 1.6180339887498948482

namespace rabbit
{
	// Класс решает задачу оценки интервала поиска для последующего применения
	// алгоритмов однопараметрической оптимизации.
	class minimize_bracket
	{
	public:
		rabbit::function * func = nullptr;

		real a = 0;
		real b = 0;
		real c = 0;

		real fa = 0;
		real fb = 0;
		real fc = 0;

		real lambda = GOLD;
		real min_border = std::numeric_limits<real>::lowest();
		real max_border = std::numeric_limits<real>::max();

		int maxiter = 10;

	public:
		minimize_bracket(rabbit::function * func, real a, real b) :
			func(func),
			a(a),
			b(b)
		{}

		real limited(real c)
		{
			return c < min_border ? min_border :
			       c > max_border ? max_border :
			       c;
		}

		real strt()
		{
			return a;
		}

		real fini()
		{
			return c;
		}

		int doit()
		{
			int sts;
			auto f = func;

			if ((sts = f->value(a, &fa))) return sts;
			if ((sts = f->value(b, &fb))) return sts;

			if (fa < fb)
			{
				real tmp;
				tmp = a; a = b; b = tmp;
				tmp = fa; fa = fb; fb = tmp;
			}

			while (maxiter--)
			{
				c = b + (b - a);

				c = limited(c);
				//nos::println("A", a, b, c, fa, fb, fc);
				if ((sts = f->value(c, &fc))) return sts;

				if (c == b)
				{
					break;
				}

				if (fc >= fb)
				{
					// нашли
					break;
				}

				a = b;
				b = c;

				fa = fb;
				fb = fc;
			}

			//nos::println(a, b, c, fa, fb, fc);
			if (a > c)
			{
				real tmp;
				tmp = a; a = c; c = tmp;
				tmp = fa; fa = fc; fc = tmp;
			}


			//nos::println(a, b, c, fa, fb, fc);

			return 0;
		}
	};
}

#endif