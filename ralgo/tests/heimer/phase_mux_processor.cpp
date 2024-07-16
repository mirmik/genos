#include <doctest/doctest.h>
#include <ralgo/heimer/phase_mux_processor.h>

TEST_CASE("mux")
{
    heimer::axis_state x("x"), y("y"), z("z");
    heimer::phase3_state p("p");
    heimer::phase_mux_processor<3> phs3("phs3");

    heimer::signal_head *leftside[] = {&x, &y, &z};
    phs3.set_leftside(leftside);

    heimer::signal_head *rightside[] = {&p};
    phs3.set_rightside(rightside);
}
