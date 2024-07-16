#include <doctest/doctest.h>
#include <ralgo/util/index_brute_force.h>

TEST_CASE("index_brute_force")
{
    int arr[3];
    int sts;

    index_brute_force_init(arr, 3);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 1);
    CHECK_EQ(arr[2], 2);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 1);
    CHECK_EQ(arr[2], 3);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 1);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 2);
    CHECK_EQ(arr[2], 3);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 2);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 0);
    CHECK_EQ(arr[1], 3);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 1);
    CHECK_EQ(arr[1], 2);
    CHECK_EQ(arr[2], 3);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 1);
    CHECK_EQ(arr[1], 2);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 1);
    CHECK_EQ(arr[1], 3);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 2);
    CHECK_EQ(arr[1], 3);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, 0);

    sts = index_brute_force_next(arr, 3, 5);
    CHECK_EQ(arr[0], 2);
    CHECK_EQ(arr[1], 3);
    CHECK_EQ(arr[2], 4);
    CHECK_EQ(sts, -1);
}
