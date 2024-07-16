#include <doctest/doctest.h>
#include <nos/io/sstream.h>
#include <ralgo/cnc/feedback_guard.h>

TEST_CASE("tandem")
{
    cnc::feedback_guard guard(3);
    nos::stringstream ss;

    auto args = nos::tokens("8:5 maxmistake:25");
    guard.add_tandem_command(args, ss);
    CHECK_EQ(guard.tandems().size(), 1);
    CHECK_EQ(guard.tandems()[0].nums().size(), 1);
    CHECK_EQ(guard.tandems()[0].nums()[0], 8);
    CHECK_EQ(guard.tandems()[0].muls().size(), 1);
    CHECK_EQ(guard.tandems()[0].muls()[0], 5);
    CHECK_EQ(guard.tandems()[0].maximum_tandem_mistake(), 25);

    auto args2 = nos::tokens("Z:3 A:5 maxmistake:33");
    guard.add_tandem_command(args2, ss);
    CHECK_EQ(guard.tandems().size(), 2);
    CHECK_EQ(guard.tandems()[1].nums().size(), 2);
    CHECK_EQ(guard.tandems()[1].nums()[0], 2);
    CHECK_EQ(guard.tandems()[1].nums()[1], 3);
    CHECK_EQ(guard.tandems()[1].muls().size(), 2);
    CHECK_EQ(guard.tandems()[1].muls()[0], 3);
    CHECK_EQ(guard.tandems()[1].muls()[1], 5);
    CHECK_EQ(guard.tandems()[1].maximum_tandem_mistake(), 33);

    auto args3 = nos::tokens("X Y");
    guard.add_tandem_command(args3, ss);
    CHECK_EQ(guard.tandems().size(), 3);
    CHECK_EQ(guard.tandems()[2].nums().size(), 2);
    CHECK_EQ(guard.tandems()[2].nums()[0], 0);
    CHECK_EQ(guard.tandems()[2].nums()[1], 1);
    CHECK_EQ(guard.tandems()[2].muls().size(), 2);
    CHECK_EQ(guard.tandems()[2].muls()[0], 1);
    CHECK_EQ(guard.tandems()[2].muls()[1], 1);
    CHECK_EQ(guard.tandems()[2].maximum_tandem_mistake(),
             MAXIMUM_TANDEM_MISTAKE);

    auto args4 = nos::tokens("B:-2 C");
    guard.add_tandem_command(args4, ss);
    CHECK_EQ(guard.tandems().size(), 4);
    CHECK_EQ(guard.tandems()[3].nums().size(), 2);
    CHECK_EQ(guard.tandems()[3].nums()[0], 4);
    CHECK_EQ(guard.tandems()[3].nums()[1], 5);
    CHECK_EQ(guard.tandems()[3].muls().size(), 2);
    CHECK_EQ(guard.tandems()[3].muls()[0], -2);
    CHECK_EQ(guard.tandems()[3].muls()[1], 1);
    CHECK_EQ(guard.tandems()[3].maximum_tandem_mistake(),
             MAXIMUM_TANDEM_MISTAKE);

    guard.remove_tandem(0);
    CHECK_EQ(guard.tandems()[0].nums()[0], 8);
    CHECK_EQ(guard.tandems()[1].nums()[0], 2);
    CHECK_EQ(guard.tandems()[2].nums()[0], 4);

    auto args5 = nos::tokens("X Y");
    guard.add_tandem_command(args5, ss);
    CHECK_EQ(guard.tandems()[0].nums()[0], 8);
    CHECK_EQ(guard.tandems()[1].nums()[0], 2);
    CHECK_EQ(guard.tandems()[2].nums()[0], 4);
    CHECK_EQ(guard.tandems()[3].nums()[0], 0);
}