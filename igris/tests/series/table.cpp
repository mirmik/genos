#include <doctest/doctest.h>
#include <igris/series/block.h>
#include <igris/series/series.h>

struct table_A
{
    int a;
    double b;
    float c;
};

TEST_CASE("table.capacity")
{
    igris::series data = igris::make_series<table_A>();
    CHECK_EQ(data.count_of_blocks(), 0);

    data.reserve(15);
    data.reserve(7);

    CHECK_EQ(data.right_capacity(), 22);
    CHECK_EQ(data.count_of_blocks(), 2);
}

TEST_CASE("table.1")
{
    igris::series data = igris::make_series<table_A>();
    CHECK_EQ(data.count_of_blocks(), 0);

    data.reserve(15);
    table_A *obj = data.emplace<table_A>();

    obj->a = 33;
    obj->b = 33.05;
    obj->c = 33.04;

    auto iter = data.get_iterator(0);
    CHECK_EQ(data.blocks.next, iter.block_lnk);

    // auto ptr = iter.pointer();
}

TEST_CASE("table")
{
    igris::series data = igris::make_series<table_A>();
    CHECK_EQ(data.count_of_blocks(), 0);

    data.reserve(15);
    table_A *obj = data.emplace<table_A>();

    obj->a = 33;
    obj->b = 33.05;
    obj->c = 33.04;

    auto iter = data.get_iterator(0);
    auto ptr = iter.pointer();

    CHECK_EQ(iter.block()->ptr, iter.pointer());
    CHECK_EQ(data.count_of_blocks(), 1);
    CHECK_EQ(data.blocks.next, iter.block_lnk);
    CHECK_EQ(ptr, obj);
    CHECK_EQ(((table_A *)ptr)->a, 33);
}

TEST_CASE("table.2")
{
    igris::series data = igris::make_series<table_A>();
    CHECK_EQ(data.count_of_blocks(), 0);

    data.reserve(15);
    table_A *obj = data.emplace<table_A>();

    obj->a = 33;
    obj->b = 33.05;
    obj->c = 33.04;

    table_A &a = data.get<table_A>(0);
    CHECK_EQ(a.a, 33);
    CHECK_EQ(a.c, 33.04f);
}

TEST_CASE("table.slice")
{
    igris::series data = igris::make_series<table_A>();
    CHECK_EQ(data.count_of_blocks(), 0);

    table_A a = {1, 2.0, 3.0};
    table_A b = {4, 5.0, 6.0};
    table_A c = {7, 8.0, 9.0};
    table_A d = {10, 11.0, 12.0};
    table_A e = {13, 14.0, 15.0};

    data.push_back(a);
    data.push_back(b);
    data.push_back(c);
    data.push_back(d);
    data.push_back(e);

    auto slice = data.slice(1, 4);
    CHECK_EQ(slice.size(), 3);

    CHECK_EQ(slice.get<table_A>(0).a, 4);
    CHECK_EQ(slice.get<table_A>(1).a, 7);
    CHECK_EQ(slice.get<table_A>(2).a, 10);
}