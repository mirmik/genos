#include <doctest/doctest.h>
#include <genos/execution_controller.h>

TEST_CASE("execution_controller")
{
    genos::execution_controller ctrl("test");
    ctrl.start_session();
    ctrl.end_session();

    CHECK_EQ(genos::execution_controller_manager::count(), 1);
}