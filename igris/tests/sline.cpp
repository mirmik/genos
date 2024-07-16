#include <doctest/doctest.h>
#include <igris/container/sline.h>
#include <igris/datastruct/sline.h>
#include <string>

using namespace std::string_literals;

TEST_CASE("sline")
{
    char buf[3];

    struct sline line;
    sline_init(&line, buf, 3);

    sline_putchar(&line, 'A');
    sline_putchar(&line, 'B');
    CHECK_EQ(std::string(sline_getline(&line)), "AB"s);

    sline_putchar(&line, 'C');
    CHECK_EQ(std::string(sline_getline(&line)), "AB"s);
}

TEST_CASE("slinexx")
{
    igris::sline line;
    line.init(3);

    line.newdata('A');
    line.newdata('B');
    CHECK_EQ(std::string(line.getline()), "AB"s);

    line.newdata('C');
    CHECK_EQ(std::string(line.getline()), "AB"s);
}