#include <doctest/doctest.h>
#include <ralgo/linalg/linalg.h>

TEST_CASE("size")
{
    CHECK_EQ(std::size(linalg::vec<double, 3>()), 3);
}
