#include <doctest/doctest.h>
#include <igris/series/fiber.h>
#include <sstream>

TEST_CASE("series_fiber")
{
    igris::series series;

    std::string str = R"(a,b,c
	1,2,3
	4,5,6
	7,8,9
)";

    std::stringstream sstr(str);
    series.parse_csv_istream(sstr);

    igris::series_fiber<double> fiber0(series, (*series.annotations())[0]);
    igris::series_fiber<double> fiber1(series, (*series.annotations())[1]);

    auto it = fiber0.get_fiber_iterator(0);
    CHECK_EQ(*it, 1);
    CHECK_EQ(*++it, 4);
    CHECK_EQ(*++it, 7);

    it = fiber1.get_fiber_iterator(0);
    CHECK_EQ(*it, 2);
    CHECK_EQ(*++it, 5);
    CHECK_EQ(*++it, 8);
}