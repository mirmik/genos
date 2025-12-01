#include "doctest/doctest.h"
#include <igris/util/macro.h>

int accum = 0;
void foo(int a) { accum += a; }

#ifndef _MSC_VER
TEST_CASE("macro")
{
    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1);
    CHECK_EQ(accum, 1);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2);
    CHECK_EQ(accum, 3);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3);
    CHECK_EQ(accum, 6);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3, 4);
    CHECK_EQ(accum, 10);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3, 4, 5);
    CHECK_EQ(accum, 15);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3, 4, 5, 6);
    CHECK_EQ(accum, 21);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3, 4, 5, 6, 7);
    CHECK_EQ(accum, 28);

    accum = 0;
    ARGS_INVOKE_FOR_EACH(foo, 1, 2, 3, 4, 5, 6, 7, 8);
    CHECK_EQ(accum, 36);
}
#endif
