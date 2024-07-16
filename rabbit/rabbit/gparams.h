#ifndef RABBIT_GPARAMS_H
#define RABBIT_GPARAMS_H

#include <rabbit/types.h>
#include <rabbit/space/pose2.h>

namespace rabbit
{
	namespace gparam
	{
		class circ
		{
			pose2  pose;
			r_float radius;

		public:
			circ(pose2 axes, r_float radius) :
				pose(axes), radius(radius)
			{}

			circ transformed_by(pose2 trsf)
			{
				return { trsf * pose, radius };
			}
		};

		class lin
		{
			pose2  pose;

			lin(const pose2& trsf)
				: pose(trsf)
			{}

			lin(vec2 apnt, vec2 bpnt)
				: pose(apnt, vector_angle(bpnt - apnt))
			{}

			lin transformed_by(pose2 trsf)
			{
				return { trsf * pose };
			}
		};
	}
}

#endif