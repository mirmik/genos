#include <doctest/doctest.h>
#include <igris/container/static_string.h>

TEST_CASE("static_string")
{
    igris::static_string<10> str;
    str.push_back('a');
    str.push_back('b');
    str.push_back('c');
    str.push_back('d');
    str.push_back('e');
    str.push_back('f');
    str.push_back('g');
    str.push_back('h');
    str.push_back('i');
    str.push_back('j');

    CHECK(str.size() == 10);
    CHECK(strcmp(str.c_str(), "abcdefghij") == 0);
}