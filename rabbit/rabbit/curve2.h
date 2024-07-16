#ifndef RABBIT_GEOM_CURVE2_H
#define RABBIT_GEOM_CURVE2_H

#include <rabbit/third/linalg.h>
#include <rabbit/interval.h>

#include <nos/print.h>
#include <nos/fprint.h>

#include <numeric>
#include <utility>

namespace rabbit
{
	namespace crv2
	{
		class curve2
		{
		public:
			virtual vec2 d0(r_float t) = 0;
			virtual vec2 d1(r_float t) = 0;

			r_float strt = -std::numeric_limits<r_float>::infinity();
			r_float fini =  std::numeric_limits<r_float>::infinity();
			bool periodic = false; 
		};

		class line : public curve2
		{
			vec2 loc;
			vec2 dir;

			line(vec2 loc, vec2 dir)
				: loc(loc), dir(dir)
			{}

			static line segment(vec2 apnt, vec2 bpnt) 
			{
				auto diff = bpnt - apnt;
				auto ln = line(apnt, normalize(diff));

				ln.strt = 0;
				ln.fini = length(diff);

				return ln;
			}

			vec2 d0(r_float t) override
			{
				return loc + dir * t;
			}

			vec2 d1(r_float t) override
			{
				(void) t;
				return dir;
			}
		};

		struct trimmed_curve 
		{

		};

	}
}

#endif