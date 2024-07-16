#include <doctest/doctest.h>
#include <igris/container/static_string.h>

TEST_CASE("static string") 
{
	igris::static_string<12> str;

	str.push_back('A');
}