#ifndef RABBIT_GEOM_CURVE_H
#define RABBIT_GEOM_CURVE_H

#include <rabbit/types.h>
#include <ralgo/space/pose3.h>

namespace rabbit
{
	class curve
	{
	protected:
		ralgo::pose3<real> _pose = {};

	public:
		curve() {};
		curve(pose3<real> pose) : _pose(pose) {}

		virtual vec3 d0(real t) = 0;
		virtual vec3 d1(real t) = 0;

		virtual real tmin() = 0;
		virtual real tmax() = 0;

		virtual ~curve() = default;
	};

	class ellipse_curve : public curve
	{
		real  _a = 0;
		real  _b = 0;

	public:
		ellipse_curve(real a, real b, ralgo::pose3<real> pose)
			: curve(pose), _a(a), _b(b)
		{}

		vec3 d0(real t) override
		{
			real x = _a * cos(t);
			real y = _b * sin(t);
			real z = 0;

			return _pose.transform(vec3(x, y, z));
		}

		vec3 d1(real t) override
		{
			real x = - _a * sin(t);
			real y =   _b * cos(t);
			real z = 0;

			return _pose.rotate(vec3(x, y, z));
		}
	};
}

#endif