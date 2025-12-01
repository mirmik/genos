#include <igris/container/chunked_vector.h>

LT_BEGIN_TEST(igris_test_suite, chunked_vector_allocator)
{
	igris::chunked_vector<int> vec(3);

	vec.push_back(1);
	CHECK(vec.size() == 1);
	vec.push_back(2);
	CHECK(vec.size() == 2);
	vec.push_back(3);
	CHECK(vec.size() == 3);
	vec.push_back(4);
	CHECK(vec.size() == 4);
	vec.push_back(5);
	CHECK(vec.size() == 5);
	vec.push_back(6);
	CHECK(vec.size() == 6);
	vec.push_back(7);
	CHECK(vec.size() == 7);

	CHECK(vec[0] == 1);
	CHECK(vec[1] == 2);
	CHECK(vec[2] == 3);
	CHECK(vec[3] == 4);
	CHECK(vec[4] == 5);

	CHECK(vec.size() == 7);

	auto plane_vec = vec.to_vector();
	CHECK(plane_vec.size() == 7);	
}
LT_END_TEST(chunked_vector_allocator)
