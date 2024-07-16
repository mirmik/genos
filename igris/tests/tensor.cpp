#include <doctest/doctest.h>
#include <igris/container/tensor.h>

TEST_CASE("marray")
{
    igris::tensor<double> arr({1, 2, 3, 4}, {2, 2});

    CHECK_EQ(arr.shape_product(), 4);
    CHECK_EQ(arr.stride()[0], 2);
    CHECK_EQ(arr.stride()[1], 1);

    {
        auto subarray = arr[0];
        CHECK_EQ(subarray.shape_product(), 2);
        CHECK_EQ(subarray.shape().size(), 1);
        CHECK_EQ(subarray.stride().size(), 1);
        CHECK_EQ(subarray({0}), 1);
        CHECK_EQ(subarray({1}), 2);
    }

    {
        auto subarray = arr[1];
        CHECK_EQ(subarray.shape_product(), 2);
        CHECK_EQ(subarray.shape().size(), 1);
        CHECK_EQ(subarray.stride().size(), 1);
        CHECK_EQ(subarray({0}), 3);
        CHECK_EQ(subarray({1}), 4);
    }
}

TEST_CASE("construct from vector")
{
    std::vector<double> data = {1, 2, 3, 4};
    std::vector<size_t> shape = {2, 2};
    igris::tensor<double> arr(data, shape);
}

TEST_CASE("transpose")
{
    igris::tensor<double> arr({1, 2, 3, 4}, {2, 2});

    auto transposed = arr.transpose();
    CHECK_EQ(transposed.shape_product(), 4);
    CHECK_EQ(transposed({0, 0}), 1);
    CHECK_EQ(transposed({0, 1}), 3);
    CHECK_EQ(transposed({1, 0}), 2);
    CHECK_EQ(transposed({1, 1}), 4);
}

TEST_CASE("permute")
{
    igris::tensor<double> arr({1, 2, 3, 4}, {2, 2});

    auto transposed = arr.permute({1, 0});
    CHECK_EQ(transposed.shape_product(), 4);
    CHECK_EQ(transposed({0, 0}), 1);
    CHECK_EQ(transposed({0, 1}), 3);
    CHECK_EQ(transposed({1, 0}), 2);
    CHECK_EQ(transposed({1, 1}), 4);
}

TEST_CASE("ordinal_to_storage_index")
{
    igris::tensor<double> arr({1, 2, 3, 4}, {2, 2});

    CHECK_EQ(arr.ordinal_to_storage_index(0), 0);
    CHECK_EQ(arr.ordinal_to_storage_index(1), 1);
    CHECK_EQ(arr.ordinal_to_storage_index(2), 2);
    CHECK_EQ(arr.ordinal_to_storage_index(3), 3);

    auto transposed = arr.transpose();
    CHECK_EQ(transposed.ordinal_to_storage_index(0), 0);
    CHECK_EQ(transposed.ordinal_to_storage_index(1), 2);
    CHECK_EQ(transposed.ordinal_to_storage_index(2), 1);
    CHECK_EQ(transposed.ordinal_to_storage_index(3), 3);
}

TEST_CASE("continguous")
{
    igris::tensor<double> arr({2, 2});
    arr({0, 0}) = 1;
    arr({0, 1}) = 2;
    arr({1, 0}) = 3;
    arr({1, 1}) = 4;

    auto transposed = arr.transpose();
    CHECK_EQ(transposed.shape_product(), 4);
    CHECK_EQ(transposed.stride()[0], 1);
    CHECK_EQ(transposed.stride()[1], 2);

    auto contiguous = transposed.contiguous();
    CHECK_EQ(contiguous.shape_product(), 4);
    CHECK_EQ(contiguous.stride()[0], 2);
    CHECK_EQ(contiguous.stride()[1], 1);
}

TEST_CASE("reshape")
{
    igris::tensor<double> arr({2, 2});
    arr({0, 0}) = 1;
    arr({0, 1}) = 2;
    arr({1, 0}) = 3;
    arr({1, 1}) = 4;

    arr = arr.reshape({1, 4});
    CHECK_EQ(arr.shape_product(), 4);
    CHECK_EQ(arr.stride()[0], 4);
    CHECK_EQ(arr.stride()[1], 1);
    CHECK_EQ(arr({0, 0}), 1);
    CHECK_EQ(arr({0, 1}), 2);
    CHECK_EQ(arr({0, 2}), 3);
    CHECK_EQ(arr({0, 3}), 4);

    arr = arr.reshape({4});
    CHECK_EQ(arr.shape_product(), 4);
    CHECK_EQ(arr.stride()[0], 1);
    CHECK_EQ(arr({0}), 1);
    CHECK_EQ(arr({1}), 2);
    CHECK_EQ(arr({2}), 3);
    CHECK_EQ(arr({3}), 4);
}

TEST_CASE("unsqueeze")
{
    igris::tensor<double> arrp({2, 2});
    arrp({0, 0}) = 1;
    arrp({0, 1}) = 2;
    arrp({1, 0}) = 3;
    arrp({1, 1}) = 4;

    {
        auto arr = arrp.unsqueeze(0);
        CHECK_EQ(arr.shape_product(), 4);
        CHECK_EQ(arr.shape()[0], 1);
        CHECK_EQ(arr.shape()[1], 2);
        CHECK_EQ(arr.shape()[2], 2);
        CHECK_EQ(arr.stride()[0], 1);
        CHECK_EQ(arr.stride()[1], 2);
        CHECK_EQ(arr.stride()[2], 1);
        CHECK_EQ(arr({0, 0, 0}), 1);
        CHECK_EQ(arr({0, 0, 1}), 2);
        CHECK_EQ(arr({0, 1, 0}), 3);
        CHECK_EQ(arr({0, 1, 1}), 4);
    }

    {
        auto arr = arrp.unsqueeze(1);
        CHECK_EQ(arr.shape_product(), 4);
        CHECK_EQ(arr.shape()[0], 2);
        CHECK_EQ(arr.shape()[1], 1);
        CHECK_EQ(arr.shape()[2], 2);
        CHECK_EQ(arr.stride()[0], 2);
        CHECK_EQ(arr.stride()[1], 1);
        CHECK_EQ(arr.stride()[2], 1);
        CHECK_EQ(arr({0, 0, 0}), 1);
        CHECK_EQ(arr({0, 0, 1}), 2);
        CHECK_EQ(arr({1, 0, 0}), 3);
        CHECK_EQ(arr({1, 0, 1}), 4);
    }

    {
        auto arr = arrp.unsqueeze(2);
        CHECK_EQ(arr.shape_product(), 4);
        CHECK_EQ(arr.shape()[0], 2);
        CHECK_EQ(arr.shape()[1], 2);
        CHECK_EQ(arr.shape()[2], 1);
        CHECK_EQ(arr.stride()[0], 2);
        CHECK_EQ(arr.stride()[1], 1);
        CHECK_EQ(arr.stride()[2], 1);
        CHECK_EQ(arr({0, 0, 0}), 1);
        CHECK_EQ(arr({0, 1, 0}), 2);
        CHECK_EQ(arr({1, 0, 0}), 3);
        CHECK_EQ(arr({1, 1, 0}), 4);
    }
}

TEST_CASE("resize")
{
    igris::tensor<double> arr({2, 2});
    arr({0, 0}) = 1;
    arr({0, 1}) = 2;
    arr({1, 0}) = 3;
    arr({1, 1}) = 4;

    {
        auto a = arr.resize({2, 3});
        CHECK_EQ(a.shape_product(), 6);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.shape()[1], 3);
        CHECK_EQ(a.stride()[0], 3);
        CHECK_EQ(a.stride()[1], 1);
        CHECK_EQ(a({0, 0}), 1);
        CHECK_EQ(a({0, 1}), 2);
        CHECK_EQ(a({0, 2}), 0);
        CHECK_EQ(a({1, 0}), 3);
        CHECK_EQ(a({1, 1}), 4);
        CHECK_EQ(a({1, 2}), 0);
    }

    {
        auto a = arr.resize({1, 3});
        CHECK_EQ(a.shape_product(), 3);
        CHECK_EQ(a.shape()[0], 1);
        CHECK_EQ(a.shape()[1], 3);
        CHECK_EQ(a.stride()[0], 3);
        CHECK_EQ(a.stride()[1], 1);
        CHECK_EQ(a({0, 0}), 1);
        CHECK_EQ(a({0, 1}), 2);
        CHECK_EQ(a({0, 2}), 0);
    }
}

TEST_CASE("operator[]")
{

    igris::tensor<double> arr({2, 2, 2});
    arr({0, 0, 0}) = 1;
    arr({0, 0, 1}) = 2;
    arr({0, 1, 0}) = 3;
    arr({0, 1, 1}) = 4;
    arr({1, 0, 0}) = 5;
    arr({1, 0, 1}) = 6;
    arr({1, 1, 0}) = 7;
    arr({1, 1, 1}) = 8;

    {
        auto a = arr[0];
        CHECK_EQ(a.shape_product(), 4);
        CHECK_EQ(a.shape_product(), 4);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.shape()[1], 2);
        CHECK_EQ(a.stride()[0], 2);
        CHECK_EQ(a.stride()[1], 1);
        CHECK_EQ(a({0, 0}), 1);
        CHECK_EQ(a({0, 1}), 2);
        CHECK_EQ(a({1, 0}), 3);
        CHECK_EQ(a({1, 1}), 4);
    }

    {
        auto a = arr[1];
        CHECK_EQ(a.shape_product(), 4);
        CHECK_EQ(a.shape_product(), 4);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.shape()[1], 2);
        CHECK_EQ(a.stride()[0], 2);
        CHECK_EQ(a.stride()[1], 1);
        CHECK_EQ(a({0, 0}), 5);
        CHECK_EQ(a({0, 1}), 6);
        CHECK_EQ(a({1, 0}), 7);
        CHECK_EQ(a({1, 1}), 8);
    }

    {
        auto a = arr[{0, 0}];
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.stride()[0], 1);
        CHECK_EQ(a({0}), 1);
        CHECK_EQ(a({1}), 2);
    }

    {
        auto a = arr[{0, 1}];
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.stride()[0], 1);
        CHECK_EQ(a({0}), 3);
        CHECK_EQ(a({1}), 4);
    }

    {
        auto a = arr[{1, 0}];
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape_product(), 2);
        CHECK_EQ(a.shape()[0], 2);
        CHECK_EQ(a.stride()[0], 1);
        CHECK_EQ(a({0}), 5);
        CHECK_EQ(a({1}), 6);
    }
}