#include <doctest/doctest.h>
#include <genos/autom_schedee.h>
#include <iostream>

void foo_exec(void *data, int *state)
{
    *(int *)(data) = 1;
}

TEST_CASE("schedee_manager execution")
{
    genos::schedee_manager_init();
    int state = 0;
    genos::autom_schedee sch(foo_exec, &state);
    sch.start();

    genos::schedee_manager_step();
    CHECK(state == 1);
    CHECK_EQ(genos::schedee_list.size(), 1);
}
