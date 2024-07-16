#include <doctest/doctest.h>
#include <igris/buffer.h>

TEST_CASE("buffer")
{
    igris::buffer buf("lalala");

    CHECK_EQ(buf.size(), 6);
}
