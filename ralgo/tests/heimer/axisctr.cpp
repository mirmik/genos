#include <doctest/doctest.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/command.h>

#include <nos/print.h>

using namespace heimer;
/*
static int a = 0;

static inline
void finish_handler(void *, axis_controller *)
{
    ++a;
}

TEST_CASE("axisctr")
{
    heimer_reinit();

    int sts;

    axis_state state("state");
    axis_settings settings;
    axis_controller axctr("axctr", &settings, 1);

    CHECK_EQ(axctr.name(), "axctr");
    axctr.set_handlers(nullptr, nullptr, finish_handler);
    double gain = 1000; axctr.set_gain(&gain);
    axctr.set_velocity_external(10);
    axctr.set_accdcc_external(5, 5);
    double forw = 100, back = -100;
    axctr.set_limits_external(&back, &forw);

    axis_state * state_ptr = &state;
    axctr.set_controlled(&state_ptr);
    double tgt = 100;
    sts = axctr.incmove(0, &tgt);
    CHECK_EQ(sts, 0);

    CHECK_EQ(axctr.external_velocity(), doctest::Approx(10.f));
    CHECK_EQ(axctr.external_acceleration(), doctest::Approx(5.f));
    CHECK_EQ(axctr.external_decceleration(), doctest::Approx(5.f));

    CHECK_EQ(axctr.lintraj.ftim, 10 * discrete_time_frequency());

    CHECK_EQ(axctr.ctrpos_external(0), 0);
    axctr.serve(0);
    CHECK_EQ(axctr.ctrvel_external(0), 0);
    CHECK_EQ(axctr.ctrpos_external(0), 0);
    CHECK_EQ(a, 0);

    axctr.serve(1 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), 5);
    CHECK_EQ(a, 0);

    axctr.serve(2 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), 10);
    CHECK_EQ(a, 0);

    axctr.serve(5 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), 10);
    state.feedvel = state.ctrvel;
    CHECK_EQ(axctr.restore_internal_velocity_from_axstates(), doctest::Approx(10
/ discrete_time_frequency())); CHECK_EQ(axctr.ctrpos_external(0), 40);
    CHECK_EQ(a, 0);

    axctr.serve(6 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), 10);
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(50));
    CHECK_EQ(a, 0);

    axctr.serve(10 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(90));
    CHECK_EQ(a, 0);

    axctr.serve(11 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(5));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(97.5));
    CHECK_EQ(a, 0);

    axctr.serve(12 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(0));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(100));
    CHECK_EQ(a, 1);
}



TEST_CASE("axisctr_multiax")
{
    heimer_reinit();
    a = 0;

    int sts;

    axis_state state0("state0");
    axis_state state1("state1");
    axis_controller axctr("axctr", { &state0, &state1 });

    CHECK_EQ(axctr.settings[0].controlled, &state0);
    CHECK_EQ(axctr.settings[1].controlled, &state1);

    axctr.set_handlers(nullptr, nullptr, finish_handler);
    double gain = 1000; axctr.set_gain(&gain);
    axctr.set_velocity_external(10);
    axctr.set_accdcc_external(5, 5);

    double forw[2] = { 100,  100 };
    double back[2] = { -100, -100 };
    axctr.set_limits_external(back, forw);

    double tgt[] = { 100, 100 };
    sts = axctr.incmove(0, tgt);
    CHECK_EQ(sts, 0);

    CHECK_EQ(axctr.lintraj.ftim, doctest::Approx(10 * sqrt(2) *
discrete_time_frequency()));

    axctr.serve(0);
    CHECK_EQ(axctr.ctrvel_external(0), 0);
    CHECK_EQ(axctr.ctrpos_external(0), 0);
    CHECK_EQ(a, 0);

    axctr.serve(1 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(5 / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve(2 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10 / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve(5 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10 / sqrt(2)));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(40 / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve(6 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10 / sqrt(2)));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(50 / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve(10 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10 / sqrt(2)));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(90 / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve((16.142 - 1) * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(5  / sqrt(2)));
    CHECK_EQ(a, 0);

    axctr.serve(16.142 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(0));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(100));
    CHECK_EQ(a, 1);
}


TEST_CASE("axisctr_stop")
{
    heimer_reinit();
    a = 0;

    int sts;

    axis_state state("state");
    axis_settings settings[1];
    axis_controller axctr("axctr", settings, 1);

    axctr.set_handlers(nullptr, nullptr, finish_handler);
    double gain = 1000; axctr.set_gain(&gain);
    axctr.set_velocity_external(10);
    axctr.set_accdcc_external(5, 5);

    double forw[2] = { 100,  100 };
    double back[2] = { -100, -100 };
    axctr.set_limits_external(back, forw);

    axis_state * states[] = { &state };
    axctr.set_controlled(states);

    double tgt[] = { 100, 100 };
    sts = axctr.incmove(0, tgt);
    CHECK_EQ(sts, 0);

    CHECK_EQ(axctr.lintraj.ftim, doctest::Approx(10 *
discrete_time_frequency()));

    axctr.serve(0);
    CHECK_EQ(axctr.ctrvel_external(0), 0);
    CHECK_EQ(axctr.ctrpos_external(0), 0);
    CHECK_EQ(a, 0);

    axctr.serve(1 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(5));
    CHECK_EQ(a, 0);

    axctr.serve(2 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10));
    CHECK_EQ(a, 0);

    axctr.serve(5 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(40));
    CHECK_EQ(a, 0);

    state.feedvel = state.ctrvel;
    state.feedpos = state.ctrpos;

    sts = axctr.stop(5 * discrete_time_frequency());
    CHECK_EQ(sts, 0);
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(10));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(40));
    CHECK_EQ(a, 0);

    axctr.serve(5 * discrete_time_frequency());
    CHECK_EQ(axctr.restore_internal_velocity_from_axstates(), doctest::Approx(
10 / discrete_time_frequency())); CHECK_EQ(axctr.ctrvel_external(0),
doctest::Approx(10)); CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(40));
    CHECK_EQ(a, 0);

    axctr.serve(6 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(5));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(47.5));
    CHECK_EQ(a, 0);

    axctr.serve(7 * discrete_time_frequency());
    CHECK_EQ(axctr.ctrvel_external(0), doctest::Approx(0));
    CHECK_EQ(axctr.ctrpos_external(0), doctest::Approx(50));
    CHECK_EQ(a, 1);
}
*/