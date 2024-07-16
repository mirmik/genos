#include <doctest/doctest.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/heimer/command.h>
#include <ralgo/heimer/velocity_applier.h>
#include <ralgo/robo/stepper_controller.h>

using namespace robo;
using namespace heimer;
/*
TEST_CASE("stepctr")
{
    int sts;
    stepper stepsim;
    stepper_controller stepctr(&stepsim);

    stepctr.set_units_in_step(2000);

    CHECK_EQ(stepsim.steps_count(), 0);

    sts = stepctr.shift(1250);
    CHECK_EQ(sts, 0);
    CHECK_EQ(stepctr.virtual_pos(), 1250);
    CHECK_EQ(stepsim.steps_count(), 0);

    stepctr.shift(1250);
    CHECK_EQ(stepsim.steps_count(), 1);

    stepctr.shift(-1250);
    CHECK_EQ(stepsim.steps_count(), 1);

    stepctr.shift(-1250);
    CHECK_EQ(stepsim.steps_count(), 0);
}

TEST_CASE("fixed_frequency_stepctr")
{
    stepper stepsim;
    fixed_frequency_stepper_controller stepctr(&stepsim);

    stepctr.set_units_in_step(10000);
    stepctr.set_frequency(0.1);

    CHECK_EQ(stepctr.speed_to_shift, 10000 * 0.1);

    // Один импульс за дискрет
    stepctr.set_velocity(1);

    CHECK_EQ(stepctr.current_shift, 1000);

    for (int i = 0; i < 10 * 1000; ++i)
    {
        int sts = stepctr.constant_frequency_serve();
        CHECK_EQ(sts, 0);
    }

    CHECK_EQ(stepctr.control_pos(), 10000 * 1000);
    CHECK_EQ(stepctr.virtual_pos(), 10000 * 1000);
    CHECK_EQ(stepsim.steps_count(), 1000);

    // Один импульс за дискрет
    stepctr.set_velocity(-1);

    for (int i = 0; i < 10 * 1000; ++i)
    {
        stepctr.constant_frequency_serve();
    }
    CHECK_EQ(stepsim.steps_count(), 0);
}


TEST_CASE("stepctr_applier")
{
    int sts;
    heimer::heimer_reinit();

    stepper stepsim;
    fixed_frequency_stepper_controller stepctr(&stepsim);

    axis_state x("x");
    axis_controller xctr("xctr", { &x });

    velocity_applier applier("xapply", &stepctr, &x);

    stepctr.set_units_in_step((1<<20));
    stepctr.set_frequency(0.01);

    applier.set_gear(4194.304);
    applier.set_compkoeff_timeconst(0.5);

    xctr.set_velocity_external(1);
    xctr.set_acceleration_external(1);
    xctr.set_decceleration_external(1);
    double forw = 100, back = -100;
    xctr.set_limits_external(&back, &forw);

    applier.feedback(0);
    xctr.feedback(0);

    CHECK_EQ(x.feedpos, 0);
    CHECK_EQ(x.feedvel, 0);

    CHECK_EQ(xctr.external_velocity(), 1);
    sts = xctr.absmove(0, {100.});
    CHECK_EQ(xctr.is_active(), true);
    CHECK_EQ(sts, 0);

    int ii =0;
    for (int sec = 0; sec < 1; ++sec)
    {
        for (int i = 0; i < 100; ++i)
        {
            int curtime = sec * 1000 + i * 10;

            applier.feedback(curtime);
            xctr.feedback(curtime);
            xctr.serve(curtime);
            applier.serve(curtime);

            for (int j = 0; j < 1000; ++j)       // freq = 0.01
            {
                ++ii;
                sts = stepctr.constant_frequency_serve();
                CHECK_EQ(sts, 0);
            }
        }
    }
    int curtime = 1000;
    applier.feedback(curtime);
    xctr.feedback(curtime);
    xctr.serve(curtime);
    applier.serve(curtime);

    CHECK_EQ(x.ctrpos, doctest::Approx(0.5));
    CHECK_EQ(x.ctrvel, doctest::Approx(1. / discrete_time_frequency()));
    CHECK_EQ(x.feedvel, doctest::Approx(1. / discrete_time_frequency()));
    CHECK_EQ(x.feedpos, doctest::Approx(0.5).epsilon(0.001));

    //CHECK_EQ(applier.impulses_per_disc,
doctest::Approx(4.194304).epsilon(0.01));

    CHECK_EQ(applier.compspd, doctest::Approx(1. / discrete_time_frequency()));
    CHECK_EQ(stepctr.units_in_step, doctest::Approx((1<<20)));
    CHECK_EQ(stepctr.speed_to_shift, doctest::Approx((1<<20) * 0.01));

    CHECK_EQ(stepsim.steps_count(), doctest::Approx(4194/2).epsilon(1));
}
*/