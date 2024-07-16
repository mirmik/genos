#include <doctest/doctest.h>
#include <nos/fprint.h>

TEST_CASE("spec.align.left")
{
	auto str = nos::format("{:>8}", std::string("lala"));
	CHECK_EQ(str, "    lala");
}

TEST_CASE("spec.align.center")
{
	auto str = nos::format("{:^8}", std::string("lala"));
	CHECK_EQ(str, "  lala  ");
}

TEST_CASE("spec.align.right")
{
	auto str = nos::format("{:<8}", std::string("lala"));
	CHECK_EQ(str, "lala    ");
}

TEST_CASE("spec.float.1")
{
	auto str = nos::format("{:>8.2}", 55.);
	CHECK_EQ(str, "   55.00");
}

TEST_CASE("spec.float.2")
{
	auto str = nos::format("{:<8.2}", 55.50341234);
	CHECK_EQ(str, "55.50   ");
}
