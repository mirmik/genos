#include <doctest/doctest.h>
#include <igris/getopt/cliopts.h>

TEST_CASE("cliopts")
{
    const char *argv[] = {
        "test",
        "--debug",
    };

    igris::cliopts cli;
    cli.add_option("debug", 'd');

    cli.parse(sizeof(argv) / sizeof(const char *), (char **)argv);

    CHECK_EQ(cli.get_option("debug").unwrap(), true);
}

TEST_CASE("cliopts")
{
    const char *argv[] = {"test", "--str", "lalala"};

    igris::cliopts cli;
    cli.add_string("str", 's', "gagaga");

    cli.parse(sizeof(argv) / sizeof(const char *), (char **)argv);

    CHECK_EQ(cli.get_string("str").unwrap(), "lalala");
}

TEST_CASE("cliopts")
{
    const char *argv[] = {
        "test",
    };

    igris::cliopts cli;
    cli.add_string("str", 's', "gagaga");

    cli.parse(sizeof(argv) / sizeof(const char *), (char **)argv);

    CHECK_EQ(cli.get_string("str").unwrap(), "gagaga");
}
