#include <doctest/doctest.h>
#include <igris/container/static_vector.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>

TEST_CASE("static_vector") 
{
	igris::static_vector<double, 8> vec1 {1,2,3};
	igris::static_vector<double, 8> vec2 {1,2,3,4,5};

	CHECK_EQ(vec1.size(), 3);
	CHECK_EQ(vec2.size(), 5);

	vec2 = vec1;
	CHECK_EQ(vec2.size(), 3);
	CHECK_EQ(vec2[0], 1);
	CHECK_EQ(vec2[1], 2);
	CHECK_EQ(vec2[2], 3);

	vec2.resize(2);
	CHECK_EQ(vec2.size(), 2);
}

TEST_CASE("vector -> static_vector") 
{
	std::vector<double> vec {4,5,6};
	igris::static_vector<double, 8> svec {1,2,3};

	svec.clear();
	std::copy(vec.begin(), vec.end(), std::back_inserter(svec));

	CHECK_EQ(svec[0], 4);
	CHECK_EQ(svec[2], 6);
}