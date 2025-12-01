
#include <igris/dprint.h>
#include <cmath>

TEST_CASE("integer")
{
	dpr(88);
	CHECK_EQ(output, "88");
	output.clear();
}

TEST_CASE("float_print")
{
	dpr(0.1);
	CHECK_EQ(output, "0.10000000");
	output.clear();
}

TEST_CASE("dpr_float_nanf")
{
	float f = std::nanf("");
	dpr(f);
	CHECK_EQ(output, "nan");
	output.clear();
}

TEST_CASE("dpr_float_infp")
{
	float f = 10e200;
	dpr(f);
	CHECK_EQ(output, "+inf");
	output.clear();
}
