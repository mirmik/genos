#include <doctest/doctest.h>
#include <igris/shell/readlinexx.h>

char buffer[48];
char hbuffer[48 * 5];

TEST_CASE("readlinexx")
{
    igris::readline rl;

    rl.init(48, 5);

    CHECK(rl.line().current_size() == 0);

    rl.newdata('a');
    CHECK(rl.line().current_size() == 1);
    CHECK(rl.line().data()[0] == 'a');

    rl.newdata('b');
    CHECK(rl.line().current_size() == 2);
    CHECK(rl.line().data()[0] == 'a');
    CHECK(rl.line().data()[1] == 'b');
}