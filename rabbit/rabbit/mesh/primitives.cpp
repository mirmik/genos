#include <rabbit/mesh/primitives.h>
#include <rabbit/geom/surface.h>

rabbit::mesh<float> rabbit::sphere_mesh(float rad, int x, int y) 
{
	return rabbit::surface_rubic_mesh<float>(rabbit::sphere_surface(rad), x, y);
}

rabbit::mesh<float> rabbit::box_mesh(float x, float y, float z) 
{
	std::vector<vec3f> vertices = {
		{-x/2,-y/2,-z/2},
		{-x/2,-y/2, z/2},
		{-x/2, y/2, z/2},
		{-x/2, y/2,-z/2},
		{ x/2,-y/2,-z/2},
		{ x/2,-y/2, z/2},
		{ x/2, y/2, z/2},
		{ x/2, y/2,-z/2}
	};

	std::vector<uvec3> triangles = {
		{0,1,2},
		{0,2,3},
		{4,5,6},
		{4,6,7},
		{0,4,7},
		{0,7,3},
		{1,5,6},
		{1,6,2},
		{0,1,5},
		{0,5,4},
		{3,2,6},
		{3,6,7}		
	};

	rabbit::mesh box(vertices, triangles);
	return box;
}

rabbit::mesh<float> rabbit::cylinder_mesh(float r, float h, int x, int y) 
{
	auto surface = rabbit::cylinder_surface(r, h);
	return rabbit::surface_rubic_mesh<float>(surface, 0, M_PI*2, x, 0, h, y);
}