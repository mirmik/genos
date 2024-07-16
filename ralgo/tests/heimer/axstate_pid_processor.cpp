#include <doctest/doctest.h>
#include <ralgo/heimer/axis_stub_processor.h>
#include <ralgo/heimer/axstate_pid_processor.h>
#include <ralgo/heimer/command.h>

/*
TEST_CASE("axstate_pid_processor")
{
    heimer::heimer_reinit();

    heimer::axis_state x("x");
    heimer::axis_stub_processor xstub("xstub");
    heimer::scalar_signal t("t");

    xstub.bind(&x);
    xstub.apply_speed_mode(true);

    heimer::axstate_pid_processor proc("pidproc");
    proc.set_left(&x);
    proc.set_right(&t);
    proc.set_compcoeff(0.1);

    t.value = 10;

    proc.activate(0);

    xstub.feedback(0);
    proc.serve(0);
    xstub.serve(0);
    CHECK_EQ(x.feedpos, 0);
    CHECK_EQ(x.ctrvel, doctest::Approx(1));

    int i = 1;
    for (; i < 200; ++i)
    {
        xstub.feedback(i);
        proc.serve(i);
        xstub.serve(i);
    }

    CHECK_EQ(x.ctrpos, doctest::Approx(10));
}
*/