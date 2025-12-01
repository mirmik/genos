#include <doctest/doctest.h>

#if __cplusplus > 201703L && __has_include(<concepts>)
#include <igris/container/ndarray.h>

TEST_CASE("2darray")
{
    igris::ndarray<double> arr = {
        {10., 30., 50.},
        {10., 20., 30.},
    };

    CHECK_EQ(arr.dim(), 2);
    CHECK_EQ(arr.shape()[0], 2);
    CHECK_EQ(arr.shape()[1], 3);
    CHECK_EQ(arr.shape().size(), 2);

    CHECK_EQ(arr.planed_index({0, 0}), 0);
    CHECK_EQ(arr({0, 0}), 10);
    CHECK_EQ(arr({0, 1}), 30);
    CHECK_EQ(arr({0, 2}), 50);
    CHECK_EQ(arr({1, 0}), 10);
    CHECK_EQ(arr({1, 1}), 20);
    CHECK_EQ(arr({1, 2}), 30);

    arr.reshape({6});
    CHECK_EQ(arr.shape()[0], 6);
    CHECK_EQ(arr.shape().size(), 1);
    CHECK_EQ(arr.dim(), 1);
    CHECK_EQ(arr({0}), 10);
    CHECK_EQ(arr({1}), 30);
    CHECK_EQ(arr({2}), 50);
    CHECK_EQ(arr({3}), 10);
    CHECK_EQ(arr({4}), 20);
    CHECK_EQ(arr({5}), 30);
}

TEST_CASE("2darray shape constructor")
{
    igris::ndarray<double> arr({10., 30., 50., 10., 20., 30.}, {2, 3});
    CHECK_EQ(arr.dim(), 2);
    CHECK_EQ(arr.shape(), std::vector<size_t>{2, 3});
    CHECK_EQ(arr.planed_index({0, 0}), 0);
    CHECK_EQ(arr({0, 0}), 10);
}

TEST_CASE("3darray")
{
    igris::ndarray<double> arr = {
        {{10, 20}, {30, 60}, {50, 100}},
        {{10, 20}, {20, 40}, {30, 60}},
    };

    CHECK_EQ(arr.dim(), 3);
    CHECK_EQ(arr.shape()[0], 2);
    CHECK_EQ(arr.shape()[1], 3);
    CHECK_EQ(arr.shape()[2], 2);
    CHECK_EQ(arr.shape().size(), 3);

    CHECK_EQ(arr.planed_index({0, 0, 0}), 0);
    CHECK_EQ(arr.planed_index({0, 0, 1}), 1);
    CHECK_EQ(arr.planed_index({1, 0, 1}), 7);
    CHECK_EQ(arr({0, 0, 0}), 10);
    CHECK_EQ(arr({0, 1, 0}), 30);
    CHECK_EQ(arr({0, 2, 0}), 50);
    CHECK_EQ(arr({1, 0, 0}), 10);
    CHECK_EQ(arr({1, 1, 0}), 20);
    CHECK_EQ(arr({1, 2, 0}), 30);
    CHECK_EQ(arr({0, 0, 1}), 20);
    CHECK_EQ(arr({0, 1, 1}), 60);
    CHECK_EQ(arr({0, 2, 1}), 100);
    CHECK_EQ(arr({1, 0, 1}), 20);
    CHECK_EQ(arr({1, 1, 1}), 40);
    CHECK_EQ(arr({1, 2, 1}), 60);
}

TEST_CASE("ndarray.operator+")
{
    igris::ndarray<double> arr1 = {10., 20.};
    igris::ndarray<double> arr2 = {1., 3.};
    igris::ndarray<double> arr3 = arr1 + arr2;

    CHECK_EQ(arr3, igris::ndarray<double>{11, 23});
}

TEST_CASE("ndarray.operator-")
{
    igris::ndarray<double> arr1 = {10., 20.};
    igris::ndarray<double> arr2 = {1., 3.};
    igris::ndarray<double> arr3 = arr1 - arr2;

    CHECK_EQ(arr3, igris::ndarray<double>{9, 17});
}

TEST_CASE("ndarray.vectored")
{
    igris::ndarray<igris::ndarray<double>> arr1 = {
        igris::ndarray<double>{10., 11.}, igris::ndarray<double>{20., 21.}};

    auto a = arr1({0});
    // auto b = arr1[0];

    CHECK_EQ(arr1.dim(), 1);
    CHECK_EQ(arr1({0}).dim(), 1);
    // CHECK_EQ(a, b);
    CHECK_EQ(a, igris::ndarray<double>{10, 11});
    // CHECK_EQ(b, igris::ndarray<double>{10, 11});
}

TEST_CASE("ndarray.get")
{
    igris::ndarray<double> arr = {
        {10., 30., 50.},
        {10., 20., 30.},
    };

    auto vals = arr.get({{0, 2}, {1, 1}});
    CHECK_EQ(vals, std::vector<double>{50, 20});
}

#endif