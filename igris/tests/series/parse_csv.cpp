#include <doctest/doctest.h>
#include <igris/series/series.h>

#include <sstream>

TEST_CASE("csv_parse_istream")
{
    igris::series series;

    std::string str = R"(a,b,c
	1,2,3
	4,5,6
	7,8,9
)";

    std::stringstream sstr(str);
    series.parse_csv_istream(sstr);

    auto el0 = series.get_iterator(0);
    auto el1 = series.get_iterator(1);
    auto el2 = series.get_iterator(2);

    CHECK_EQ(series.size(), 3);
    CHECK_EQ(series.annotator().annotations()->size(), 3);

    auto el0view = el0.view<double[3]>();
    auto el1view = el1.view<double[3]>();
    auto el2view = el2.view<double[3]>();

    CHECK_EQ(el0view[0], 1);
    CHECK_EQ(el0view[1], 2);
    CHECK_EQ(el0view[2], 3);

    CHECK_EQ(el1view[0], 4);
    CHECK_EQ(el1view[1], 5);
    CHECK_EQ(el1view[2], 6);

    CHECK_EQ(el2view[0], 7);
    CHECK_EQ(el2view[1], 8);
    CHECK_EQ(el2view[2], 9);
}