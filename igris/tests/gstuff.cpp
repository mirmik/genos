#include <doctest/doctest.h>
#include <igris/protocols/gstuff.h>

TEST_CASE("gstuffing")
{
    iovec arr[3] = {
        {(void *)"hello", 5},
        {(void *)"world", 5},
        {(void *)"!", 1},
    };

    auto sbuffer = gstuffing_v(arr, 3);

    CHECK(sbuffer.size() == 14);
}