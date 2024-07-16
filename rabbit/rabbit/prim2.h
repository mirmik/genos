#ifndef RABBIT_PRIM2_H
#define RABBIT_PRIM2_H

#include <rabbit/types.h>
#include <ralgo/space/pose2.h>

namespace rabbit
{
	class equation_curve2 
	{
		virtual real subs(real x, real y) = 0;
		virtual ~equation_curve2() = default;
	};

	class line2eq
	{
	public:
		real a=0, b=0, c=0;

		line2eq(real a, real b, real c)
			: a(a), b(b), c(c)
		{}

		real subs(real x, real y) 
		{
			return a*x + b*y + c;
		}

		static
		line2eq from_points(vec2 p1, vec2 p2)
		{
			vec2 d = p2 - p1;

			real dx = d.x;
			real dy = d.y;
			real x1 = p1.x;
			real y1 = p1.y;
			real x2 = p2.x;
			real y2 = p2.y;

			real a = -dy;
			real b =  dx;
			real c = x1 * y2 - y1 * x2;

			return { a, b, c };
		}

		static
		line2eq from_point_and_vector(vec2 pnt, vec2 vec)
		{
			auto x = pnt.x;
			auto y = pnt.y;

			auto a = - vec.y;
			auto b =   vec.x;
			auto c = - (a * x + b * y);

			return line2eq(a, b, c);
		}

		real subs_x(real x)
		{
			return (-a * x - c) / b;
		}

		real subs(vec2 pnt) const
		{
			return pnt.x * a + pnt.y * b + c;
		}

		/*ssize_t print_to(nos::ostream & os) const
		{
			return nos::fprint_to(os, "({}x+{}y+{}=0)", a, b, c);
		}*/

		bool equal(const line2eq & oth) 
		{
			// TODO
			return true;
		}
	};

	class segm2
	{
	public:
		vec2 apnt={};
		vec2 bpnt={};

	public:
		segm2() {}

		segm2& operator = (const segm2& oth) = default;

		segm2(vec2 apnt, vec2 bpnt)
			: apnt(apnt), bpnt(bpnt) {}

		segm2 transformed(const ralgo::pose2<real>& trsf) const
		{
			return
			{
				trsf.transform_point(apnt),
				trsf.transform_point(bpnt)
			};
		}

		line2eq line_equation() const
		{
			return line2eq::from_points(apnt, bpnt);
		}

		/*ssize_t print_to(nos::ostream & os) const
		{
			return nos::fprint_to(os, "({},{})", apnt, bpnt);
		}*/
	};


	class polysegm2
	{
	public:
		vec2 * pnts=nullptr;
		int pnts_count=0;

	public:

		polysegm2()
			: pnts(nullptr)
		{}

		polysegm2& operator=(const polysegm2&) = default;

		polysegm2(vec2 * pnts, int pnts_count)
			: pnts(pnts), pnts_count(pnts_count)
		{}
	};
}

#endif