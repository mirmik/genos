#include <doctest/doctest.h>
#include <igris/container/vector2.h>

struct A
{
    int a;
    A(int _a) : a(_a) {}
    A(const A &) = default;
};

TEST_CASE("vector2")
{

    igris::vector2<A> vec;

    vec.push_back(A(33));
    vec.push_back(A(42));
    vec.push_back(A(54));
    vec.push_back(A(78));
    vec.push_back(A(22));

    CHECK_EQ(vec.size(), 5);
    CHECK_EQ(vec[0].a, 33);
    CHECK_EQ(vec[1].a, 42);
    CHECK_EQ(vec[2].a, 54);
    CHECK_EQ(vec[3].a, 78);
    CHECK_EQ(vec[4].a, 22);

    vec.invalidate();
    CHECK_EQ(vec.size(), 0);
}