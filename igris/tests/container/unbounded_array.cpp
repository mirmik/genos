#include <doctest/doctest.h>
#include <igris/container/unbounded_array.h>
#include <vector>

TEST_CASE("igris_test_suite")
{
    igris::unbounded_array<float> arr(35);
    arr[28] = 33;
    CHECK_EQ(arr[28], 33);
}

TEST_CASE("unbounded array : vector")
{
    igris::unbounded_array<std::vector<double>> arr(4);

    arr[0].push_back(1);
    arr[0].push_back(2);

    arr[1].resize(2);
    arr[1][0] = 3;

    CHECK_EQ(arr[0][0], 1);
    CHECK_EQ(arr[0][1], 2);
    CHECK_EQ(arr[1][0], 3);
    CHECK_EQ(arr[2].size(), 0);
}