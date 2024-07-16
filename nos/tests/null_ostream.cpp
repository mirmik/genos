#include <doctest/doctest.h>
#include <nos/io/null_ostream.h>

TEST_CASE("null_ostream")
{
    nos::null_ostream nos;
    nos.write("Hello, world!", 13);
}