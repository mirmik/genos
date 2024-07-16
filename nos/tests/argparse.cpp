#include <doctest/doctest.h>
#include <nos/argparse.h>
#include <nos/fprint.h>

TEST_CASE("argparse")
{
    nos::argparse parser("testname", "Test parser");
    parser.add_argument("test", 't', "Test argument", "test", true, false);
    parser.add_argument("flag", 'f', "Test flag");
    parser.add_free_argument("files", "Test free argument", "*");

    const char *argv[] = {"test", "-f", "-t", "test", "test0", "test1"};
    auto args = parser.parse_args(6, argv);

    CHECK(args.value("test") == "test");
    CHECK(args.flag("flag"));
    CHECK(args.freeargs("files").size() == 2);
}