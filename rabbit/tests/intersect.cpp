#include <rabbit/intersect.h>
#include <doctest/doctest.h>

TEST_CASE("intersect")
{
	int sts;

	auto lin = rabbit::segm2(
	               linalg::vec<double,2>(0, 2),
	               linalg::vec<double,2>(-2, 0)
	           );

	CHECK(lin.line_equation().subs_x(0) == 2);
	CHECK(lin.line_equation().subs_x(-1) == 1);
	CHECK(lin.line_equation().subs_x(-2) == 0);

	CHECK(lin.line_equation().a / lin.line_equation().b == -1);

	auto lin2 = rabbit::segm2(
	                rabbit::vec2(-2, 1),
	                rabbit::vec2(0, 1)
	            );

	rabbit::vec2 pnt;
	sts = intersect_point_segm2_segm2(
	          &lin,
	          &lin2,
	          &pnt
	      );

	CHECK_EQ(sts, 1);
	CHECK(fabs(pnt.x + 1) < 1e-5);
	CHECK(fabs(pnt.y - 1) < 1e-5);

	lin2 = rabbit::segm2(
	           rabbit::vec2(-2, 2),
	           rabbit::vec2(0, 0)
	       );

	sts = intersect_point_segm2_segm2(
	          &lin,
	          &lin2,
	          &pnt
	      );

	CHECK_EQ(sts, 1);
}

TEST_CASE("polysegment_intersect")
{
	linalg::vec<double, 2> _psegm[] =
	{
		{0, 0},
		{0, 10},
		{10, 10},
		{10, 0},
		{0, 0},
	};

	linalg::vec<double, 2> ipnts[10];

	rabbit::segm2 segm({-1,0}, {10, 11});
	rabbit::segm2 segm0({-1,-1}, {11, 11});
	rabbit::segm2 segm1({0,0}, {10, 10});
	rabbit::polysegm2 psegm(_psegm, 5);

	int retcount;

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm, &psegm, ipnts); 
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(0,1)) == 0);
	CHECK(linalg::length(ipnts[1] - linalg::vec<double,2>(9,10)) == 0);

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm0, &psegm, ipnts); 
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(0,0)) == 0);
	CHECK(linalg::length(ipnts[1] - linalg::vec<double,2>(10,10)) == 0);

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm1, &psegm, ipnts); 
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(0,0)) == 0);
	CHECK(round(linalg::length(ipnts[1] - linalg::vec<double,2>(10,10))) == 0);
}


TEST_CASE("polysegment_intersect_nocenter")
{
	linalg::vec<double, 2> _psegm[] =
	{
		{-20, -5},
		{-20, 5},
		{-10, 5},
		{-10, -5},
		{-20, -5},
	};

	linalg::vec<double, 2> ipnts[10];

	rabbit::segm2 segm({-30,0}, {30, 0});
	rabbit::polysegm2 psegm(_psegm, 5);

	int retcount;

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm, &psegm, ipnts); 
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(-20,0)) == 0);
	CHECK(linalg::length(ipnts[1] - linalg::vec<double,2>(-10,0)) == 0);
}


TEST_CASE("polysegment_intersect_nocenter2")
{
	linalg::vec<double, 2> _psegm[] =
	{
		{-5, 10},
		{ 5, 10},
		{ 5, 20},
		{-5, 20},
		{-5, 10},
	};

	linalg::vec<double, 2> ipnts[10];

	rabbit::segm2 segm({0,-30}, {0, 30});
	rabbit::polysegm2 psegm(_psegm, 5);

	int retcount;

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm, &psegm, ipnts); 
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(0,10)) == 0);
	CHECK(linalg::length(ipnts[1] - linalg::vec<double,2>(0,20)) == 0);
}


TEST_CASE("polysegment_intersect_nocenter2_opposite")
{
	linalg::vec<double, 2> _psegm[] =
	{
		{-5, 10},
		{-5, 20},
		{ 5, 20},
		{ 5, 10},
		{-5, 10},
	};

	linalg::vec<double, 2> ipnts[10];

	rabbit::segm2 segm({0,-30}, {0, 30});
	rabbit::polysegm2 psegm(_psegm, 5);

	int retcount;

	retcount = rabbit::intersect_points_segm2_polysegm2(&segm, &psegm, ipnts); 
	
	CHECK_EQ(retcount, 2);
	CHECK(linalg::length(ipnts[0] - linalg::vec<double,2>(0,20)) == 0);
	CHECK(linalg::length(ipnts[1] - linalg::vec<double,2>(0,10)) == 0);
}