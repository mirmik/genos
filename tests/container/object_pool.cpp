#include <doctest/doctest.h>
#include <igris/container/static_object_pool.h>
#include <vector>

TEST_CASE("object_pool : 1")
{
    igris::static_object_pool<int, 4> pool;
    CHECK_EQ(pool.avail(), 4);

    int *a = pool.create(5);
    int *b = pool.create(6);
    int *c = pool.create(7);

    CHECK(*a == 5);
    CHECK(*b == 6);
    CHECK(*c == 7);

    CHECK_EQ(pool.avail(), 1);

    pool.destroy(a);
    pool.destroy(b);
    pool.destroy(c);

    CHECK_EQ(pool.avail(), 4);

    int *d = pool.create(8);
    int *e = pool.create(9);
    int *f = pool.create(10);
    int *g = pool.create(11);

    int *h = pool.create(12);

    CHECK_EQ(pool.avail(), 0);

    CHECK(*d == 8);
    CHECK(*e == 9);
    CHECK(*f == 10);
    CHECK(*g == 11);
    CHECK(h == nullptr);
}