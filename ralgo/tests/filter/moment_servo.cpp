#include <doctest/doctest.h>
#include <ralgo/filter/moment_servo_filter.h>

#include <nos/print.h>
#include <ralgo/heimer/axisctr.h>
#include <ralgo/trajectory/linetraj.h>

class mass_model
{
public:
    double mass = 1;
    double pos = 0;
    double vel = 0;

    void serve(double torq, double delta)
    {
        pos += vel * delta;
        vel += torq / mass * delta;
    }
};

TEST_CASE("moment_servo_vel")
{
    mass_model model;
    ralgo::moment_servo_filter serv;

    double target = 10;

    serv.setup_velocity_parameters(0.5, 0.75, 1);

    double delta = 0.01;
    for (int i = 0; i < 300; ++i)
    {
        double error = target - model.vel;
        double torq = serv.velocity_control(error, delta);

        model.serve(torq, delta);
    }

    CHECK_EQ(model.vel, doctest::Approx(10).epsilon(1e-2));
}

TEST_CASE("moment_servo_pos")
{
    mass_model model;
    ralgo::moment_servo_filter serv;

    double target = 10;

    serv.setup_velocity_parameters(0.1, 0.75, 1);
    serv.setup_position_parameters(0.5, 0.75, 1);

    double delta = 0.01;
    for (int i = 0; i < 300; ++i)
    {
        double error_pos = target - model.pos;
        double error_vel = 0 - model.vel;
        double torq = serv.position_control(error_pos, error_vel, delta);

        model.serve(torq, delta);
    }

    CHECK_EQ(model.pos, doctest::Approx(10).epsilon(1e-2));
}

TEST_CASE("moment_servo_vel2")
{
    mass_model model;
    ralgo::moment_servo_filter serv;
    lintraj<1> ltraj;

    position_t curpos;
    velocity_t curvel;

    ltraj.init_nominal_speed(0, 3000, 0, 10, 300, 300);

    serv.setup_velocity_parameters(0.05, 0.75, 1);
    serv.setup_position_parameters(0.1, 0.75, 1);

    double delta = 0.01;
    for (int i = 0; i < 400; ++i)
    {
        ltraj.attime(delta * i * discrete_time_frequency(), &curpos, &curvel);

        double error_pos = curpos - model.pos;
        double error_vel = curvel - model.vel;
        double torq = serv.position_control(error_pos, error_vel, delta);

        model.serve(torq, delta);
    }

    CHECK_EQ(model.pos, doctest::Approx(10).epsilon(1e-2));
}
