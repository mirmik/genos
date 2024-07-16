#ifndef RABBIT_SPACE_ACCEL_H
#define RABBIT_SPACE_ACCEL_H

#include <rabbit/space/inertia.h>
#include <rabbit/space/force.h>

namespace rabbit 
{
	struct haccel3 : public screw3 {};
	struct haccel2 : public screw2 {};

	haccel3<T> operator *(const inertia3& I, const force3& F) 
	{
		linalg::vec<T,3> ang = I.moment * F.ang;
		linalg::vec<T,3> lin = I.mass * F.lin;
	
		return {ang, lin};		
	}

	haccel2<T> operator *(const inertia2& I, const force2& F) 
	{
		T ang = I.moment * F.ang;
		linalg::vec<T,2> lin = I.mass * F.lin;
	
		return {ang, lin};		
	}
}

#endif