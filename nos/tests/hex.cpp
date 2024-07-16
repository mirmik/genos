#include <doctest/doctest.h>
#include <nos/io/sstream.h>
#include <nos/print.h>

TEST_CASE("printhex")
{
    nos::stringstream writer;
    nos::current_ostream = &writer;
    nos::printhex((uint16_t)0xABCD);
    CHECK_EQ(writer.str(), "ABCD");
}

TEST_CASE("printhex")
{
    nos::stringstream writer;
    nos::current_ostream = &writer;
    nos::printhex((uint32_t)0xABCD);
    CHECK_EQ(writer.str(), "0000ABCD");
}