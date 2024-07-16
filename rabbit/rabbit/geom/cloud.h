#ifndef RABBIT_POINT_CLOUD_H
#define RABBIT_POINT_CLOUD_H

#include <rabbit/types.h>

namespace rabbit 
{
	class cloud 
	{
		std::vector<vec3> _points;

	public:
		cloud(std::vector<vec3> && points) : _points(std::move(points)) {}
		cloud(const std::vector<vec3> & points) : _points(points) {}
	};
}

#endif