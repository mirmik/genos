#include <array>
#include <doctest/doctest.h>
#include <igris/container/static_vector.h>
#include <igris/shell/callable_collection.h>

TEST_CASE("callable_collection")
{
    igris::static_callable_collection<int(int, int), 10> cc;

    cc.add("sum", "Add two numbers", [](int a, int b) -> int { return a + b; });

    auto *fptr = cc.find("sum");
    CHECK((*fptr)(1, 2) == 3);
}
