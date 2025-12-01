#include <doctest/doctest.h>
#include <igris/util/string.h>

TEST_CASE("string.replace0")
{
    const char *input = "Hello abc worabcldabc";
    const char *output = "Hello  world";
    char buffer[64];

    replace_substrings(buffer, 64, input, strlen(input), "abc", 3, "", 0);

    CHECK_EQ(std::string(buffer), std::string(output));
}

TEST_CASE("string.replace1")
{
    const char *input = "Hello abc worabcldabc";
    char buffer[64];

    replace_substrings(buffer, 64, input, strlen(input), "", 0, "a", 1);

    CHECK_EQ(std::string(buffer), std::string(input));
}

TEST_CASE("string.replace2")
{
    std::string input = "Hello abc worabcldabc";

    auto output = igris::replace(input, "", "a");

    CHECK_EQ(input, output);
}

TEST_CASE("string.replace2")
{
    std::string input = "Hello abc worabcldabc";

    auto output = igris::replace(input, "abc", "");

    CHECK_EQ(output, "Hello  world");
}

TEST_CASE("string.trim")
{
    std::string input = "      Hello abc worabcldabc    \r\n\t";
    auto output = igris::trim(input);
    CHECK_EQ(output, "Hello abc worabcldabc");
}

TEST_CASE("string.trim")
{
    std::string input = "";
    auto output = igris::trim(input);
    CHECK_EQ(output, "");
}

TEST_CASE("string.trim")
{
    std::string input = "                      ";
    auto output = igris::trim(input);
    CHECK_EQ(output, "");
}

TEST_CASE("s")
{
    std::string input = "s";
    auto output = igris::trim(input);
    CHECK_EQ(output, "s");
}

TEST_CASE("string.split_cmdargs.1")
{
    std::string input = "Hello abc worabcldabc";
    auto output = igris::split_cmdargs(input);
    CHECK_EQ(output.size(), 3);
    CHECK_EQ(output[0], "Hello");
    CHECK_EQ(output[1], "abc");
    CHECK_EQ(output[2], "worabcldabc");
}

TEST_CASE("string.split_cmdargs.2")
{
    std::string input = "Hello abc 'wocasdc fcasdcas rabcldabc'";
    auto output = igris::split_cmdargs(input);
    CHECK_EQ(output.size(), 3);
    CHECK_EQ(output[0], "Hello");
    CHECK_EQ(output[1], "abc");
    CHECK_EQ(output[2], "wocasdc fcasdcas rabcldabc");
}

TEST_CASE("string.split_cmdargs.3")
{
    std::string input = "Hello \"casdcasdc scdasdcasd 'casdcasdc ascasdc'\" "
                        "'wocasdc fcasdcas rabcldabc'";
    auto output = igris::split_cmdargs(input);
    CHECK_EQ(output.size(), 3);
    CHECK_EQ(output[0], "Hello");
    CHECK_EQ(output[1], "casdcasdc scdasdcasd 'casdcasdc ascasdc'");
    CHECK_EQ(output[2], "wocasdc fcasdcas rabcldabc");
}