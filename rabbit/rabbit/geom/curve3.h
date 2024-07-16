#ifndef RABBIT_GEOM_CURVE3_H
#define RABBIT_GEOM_CURVE3_H

#include <rabbit/geom/linalg.h>

namespace rabbit
{
	namespace curve3
	{
		template <class T>
		struct curve
		{
			virtual rabbit::pnt3<T> d0(T t);
			virtual rabbit::vec3<T> d1(T t);
		};

		template <class T>
		struct ellipse
		{
			rabbit::pnt<T, 3> 	c;
			rabbit::vec<T, 3> 	x;
			rabbit::vec<T, 3> 	y;

			/*ellipse(
			    linalg::vec<T, 3> c,
			    linalg::vec<T, 3> x)
				:
				c(c), x(x), y(ort(x))
			{}*/

			ellipse(
			    linalg::vec<T, 3> c,
			    linalg::vec<T, 3> x,
			    linalg::vec<T, 3> y)
				:
				c(c), x(x), y(y)
			{}

			linalg::vec<T, 3> d0(T t) override
			{
				return c + x * (T)cos(t) + y * (T)sin(t);
			}

			linalg::vec<T, 3> d1(T t) override
			{
				return x * (T)sin(t) + y * (T)cos(t);
			}

			template <class Trans>
			ellipse transform(const Trans& trans)
			{
				return ellipse
				{
					trans.transform(c),
					trans.rotate(x),
					trans.rotate(y)
				}
			}

			size_t print_to(nos::ostream & o) const
			{
				return nos::fprint_to(o, "ellipse2(c:{},x:{},y:{})", c, x, y);
			}
		};
	}
}

#endif