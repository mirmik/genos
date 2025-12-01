#include <doctest/doctest.h>
#include <igris/tuple.h>

TEST_CASE("tuple")
{
    igris::tuple<float, int, int> tpl{0.3f, 1, 2};

    CHECK_EQ(std::get<0>(tpl), 0.3f);
    CHECK_EQ(std::get<1>(tpl), 1);
    CHECK_EQ(std::get<2>(tpl), 2);
}
