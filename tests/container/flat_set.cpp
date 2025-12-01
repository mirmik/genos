#include <doctest/doctest.h>
#include <igris/container/flat_map.h>
#include <igris/container/flat_set.h>

TEST_CASE("flat_set")
{
    igris::flat_set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    set.insert(4);
    set.insert(5);
    set.insert(6);
    set.insert(7);

    CHECK(set.size() == 7);
}