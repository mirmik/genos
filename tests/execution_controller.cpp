#include <doctest/doctest.h>
#include <genos/execution_monitor.h>

TEST_CASE("execution_monitor")
{
    genos::execution_monitor ctrl("test");
    ctrl.start_session();
    ctrl.end_session();

    CHECK_EQ(genos::execution_monitor_manager::count(), 1);
}