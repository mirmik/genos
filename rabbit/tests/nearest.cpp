#include <doctest/doctest.h>
#include <rabbit/util.h>

TEST_CASE("nearest") 
{
	linalg::vec<double,2> arr[] = 
	{
		{1, 1},
		{1, 0},
		{0.5, 0.5}
	};

	auto it = rabbit::nearest_point_from_array(
			linalg::vec<double,2>{0,0}, 
			std::begin(arr),
			std::end(arr)
		);

	CHECK_EQ(std::distance(std::begin(arr), it), 2);
}