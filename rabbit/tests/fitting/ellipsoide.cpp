#include <doctest/doctest.h>
#include <rabbit/fitting/ellipse_fit.h>
#include <rabbit/types.h>

#include <rabbit/geom/surface.h>
#include <ralgo/linspace.h>

using namespace linalg::ostream_overloads;

TEST_CASE("ellipsoide_fit") 
{
/*	std::vector<linalg::vec<float,2>> point_cloud;

	//rabbit::sphere_surface surf(1);

	ralgo::linspace<float> varr(0, 2*M_PI, 100, false);
	
	for (auto v: varr)
	{	
		auto pnt = linalg::vec<float,2>{2*sinf(v), 1*cosf(v)};
		pnt = linalg::rot<float>(M_PI/6, pnt);
		pnt = pnt + linalg::vec<float,2>{20, 5};
		point_cloud.emplace_back(pnt);
	}

	rabbit::ellipse_fit fitting(point_cloud);

*/
}