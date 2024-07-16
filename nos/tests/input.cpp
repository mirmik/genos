#include <doctest/doctest.h>
#include <nos/input.h>
#include <nos/io/sstream.h>

TEST_CASE("read_from")
{
    nos::stringstream ss("Hello World!");
    CHECK(*nos::read_from(ss, 5) == "Hello");
    CHECK(*nos::read_from(ss, 6) == " World");
    CHECK(*nos::read_from(ss, 5) == "!");
}

TEST_CASE("readline_from")
{
    nos::stringstream ss("Hello World!\r\nHello World!\r\n");
    CHECK(*nos::readline_from(ss, 100, false) == "Hello World!");
    CHECK(*nos::readline_from(ss, 100, false) == "Hello World!");
}

TEST_CASE("readuntil_from")
{
    nos::stringstream ss("Hello World!\xB0Hello World!\xB1");
    CHECK(*nos::read_until_from(ss, 1024, "\xB0", false) == "Hello World!");
    CHECK(*nos::read_until_from(ss, 1024, "\xB1", false) == "Hello World!");
}

TEST_CASE("readall_from")
{
    nos::stringstream ss("Hello World!");
    CHECK(*nos::readall_from(ss) == "Hello World!");
}

TEST_CASE("readuntil_from.buffer_api")
{
    nos::stringstream ss("Hello World!\xB0Hello World!\xB1");
    char buf[1024];
    auto ret = nos::read_until_from(ss, {buf, 1024}, {"\xB0"}, false);
    CHECK_EQ(*ret, 12);
    CHECK(std::string_view(buf, ret.value()) == "Hello World!");
}