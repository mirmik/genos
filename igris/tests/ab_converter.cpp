#include <doctest/doctest.h>
#include <igris/util/ab_converter.h>

TEST_CASE("ab_converter") 
{
	igris::int_string_converter converter ({
		{ 115, "mnd" },
		{ 223, "ke" }
	});

	converter.add(10, "la");

	CHECK_EQ(10, converter["la"]);
	CHECK_EQ("la", converter[10]);
	CHECK_EQ("ke", converter[223]);
	CHECK_EQ(115, converter["mnd"]);
}


TEST_CASE("ab_converter.same_types") 
{
	igris::ab_converter<int,int> converter ({
		{ 1, 10 },
		{ 2, 20 }
	});

	CHECK_EQ(10, converter.b(1));
	CHECK_EQ(2, converter.a(20));
}