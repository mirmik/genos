#ifndef RALGO_ROBO_MOMENT_SERVO_H
#define RALGO_ROBO_MOMENT_SERVO_H

#include <ralgo/filter/pi.h>
#include <ralgo/robo/iposvel.h>

namespace ralgo
{
    class moment_servo_filter
    {
    public:
        ralgo::pi_filter vel_pi = {};
        ralgo::pi_filter pos_pi = {};

    public:
        void setup_velocity_filter(double kp, double ki)
        {
            vel_pi.init(kp, ki);
        }

        void setup_position_filter(double kp, double ki)
        {
            pos_pi.init(kp, ki);
        }

        // T**2 = 1/ki
        // 2*ksi*T = kp/ki
        void setup_velocity_parameters(double T, double ksi, double A)
        {
            double ki = A / (T * T);
            double kp = 2 * ksi * T * ki;
            setup_velocity_filter(kp, ki);
        }

        void setup_position_parameters(double T, double ksi, double A)
        {
            double ki = A / (T * T);
            double kp = 2 * ksi * T * ki;
            setup_position_filter(kp, ki);
        }

        double velocity_control(double error_vel, double delta)
        {
            double compensate_torq = vel_pi.serve(error_vel, delta);
            return compensate_torq;
        }

        double
        position_control(double error_pos, double error_vel, double delta)
        {
            double compensate_vel = pos_pi.serve(error_pos, delta);
            double control_vel = error_vel + compensate_vel;
            double compensate_torq = velocity_control(control_vel, delta);
            return compensate_torq;
        }
    };
}

#endif
