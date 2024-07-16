#include <doctest/doctest.h>
#include <ralgo/cnc/util.h>

TEST_CASE("args_to_index_value_map")
{
    auto tokens = nos::tokens("hello X:20 Y:10");
    auto argv = nos::argv(tokens);

    CHECK_EQ(argv.size(), 3);
    CHECK_EQ(argv[0], "hello");
    CHECK_EQ(argv[1], "X:20");
    CHECK_EQ(argv[2], "Y:10");

    auto ret = args_to_index_value_map(argv.without(1));
    CHECK_EQ(ret.size(), 2);
}