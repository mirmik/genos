#include <ralgo/linalg/vecops.h>
#include <ralgo/signal/sliding_array.h>

#include <algorithm>
#include <main.h>

using namespace ralgo;

TEST_CASE("sliding_array")
{
    ralgo::sliding_array<double> arr(4);
    igris::array_view<double> check;

    CHECK(vecops::equal_all(arr.window(), dvec{0, 0, 0, 0}));
    CHECK_EQ(arr.cursor, 0);

    arr.push(1);
    CHECK(vecops::equal_all(arr.window(), dvec{0, 0, 0, 1}));
    CHECK_EQ(arr.cursor, 1);

    arr.push(2);
    CHECK(vecops::equal_all(arr.window(), dvec{0, 0, 1, 2}));
    CHECK_EQ(arr.cursor, 2);

    arr.push(3);
    CHECK(vecops::equal_all(arr.window(), dvec{0, 1, 2, 3}));
    CHECK_EQ(arr.cursor, 3);

    arr.push(4);
    CHECK(vecops::equal_all(arr.window(), dvec{1, 2, 3, 4}));
    CHECK_EQ(arr.cursor, 0);

    arr.push(5);
    CHECK(vecops::equal_all(arr.window(), dvec{2, 3, 4, 5}));
    CHECK_EQ(arr.cursor, 1);

    arr.push(6);
    CHECK(vecops::equal_all(arr.window(), dvec{3, 4, 5, 6}));
    CHECK_EQ(arr.cursor, 2);

    arr.push(7);
    CHECK(vecops::equal_all(arr.window(), dvec{4, 5, 6, 7}));
    CHECK_EQ(arr.cursor, 3);

    arr.push(8);
    CHECK(vecops::equal_all(arr.window(), dvec{5, 6, 7, 8}));
    CHECK_EQ(arr.cursor, 0);

    arr.push(9);
    CHECK(vecops::equal_all(arr.window(), dvec{6, 7, 8, 9}));
    CHECK_EQ(arr.cursor, 1);

    arr.push(10);
    CHECK(vecops::equal_all(arr.window(), dvec{7, 8, 9, 10}));
    CHECK_EQ(arr.cursor, 2);

    double inarr0[] = {2, 3, 4};
    arr.push(inarr0, 3);
    CHECK(vecops::equal_all(arr.window(), dvec{10, 2, 3, 4}));
    CHECK_EQ(arr.cursor, 1);

    double inarr1[] = {5, 6};
    arr.push(inarr1, 2);
    CHECK(vecops::equal_all(arr.window(), dvec{3, 4, 5, 6}));
    CHECK_EQ(arr.cursor, 3);
}
