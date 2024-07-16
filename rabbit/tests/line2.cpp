#include <rabbit/prim2.h>
#include <doctest/doctest.h>

#include <nos/print.h>

TEST_CASE("line2")
{
	linalg::vec<double,2> pnt(0,0);
	linalg::vec<double,2> vec(1,2);

	auto aline = rabbit::line2eq::from_point_and_vector(pnt, vec);
	auto bline = rabbit::line2eq::from_points(pnt, vec);
	
	CHECK(aline.equal(bline));
}