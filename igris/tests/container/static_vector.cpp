#include <doctest/doctest.h>
#include <igris/container/static_vector.h>

TEST_CASE("static_vector")
{
    igris::static_vector<int, 15> vec;

    vec.emplace_back(33);
    vec.emplace_back(22);
    vec.emplace_back(11);

    CHECK_EQ(vec[0], 33);
    CHECK_EQ(vec[1], 22);
    CHECK_EQ(vec[2], 11);

    CHECK_EQ(vec.room(), 15 - 3);
}
