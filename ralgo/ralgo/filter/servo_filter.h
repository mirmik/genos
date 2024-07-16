#ifndef RALGO_SERVO_FILTER_H
#define RALGO_SERVO_FILTER_H

#include <ralgo/filter/pi.h>

namespace ralgo
{
    class servo_filter
    {
    public:
        ralgo::pi_filter pos_pi = {1, 0};
        ralgo::pi_filter vel_pi = {1, 0.5};
        ralgo::pi_filter torq_pi = {1, 0.5};

    public:
        void setup_velocity_filter(double kp, double ki)
        {
            vel_pi.init(kp, ki);
        }

        void setup_position_filter(double kp, double ki)
        {
            pos_pi.init(kp, ki);
        }

        void setup_torque_filter(double kp, double ki)
        {
            torq_pi.init(kp, ki);
        }

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

        double
        torque_control(double target_torq, double current_torq, double delta)
        {
            return torq_pi.serve(target_torq - current_torq, delta);
        }

        double
        velocity_control(double target_vel, double current_vel, double delta)
        {
            return vel_pi.serve(target_vel - current_vel, delta);
        }

        double
        position_control(double target_pos, double current_pos, double delta)
        {
            return pos_pi.serve(target_pos - current_pos, delta);
        }

        /*double velocity_torque_control(
            double current_vel, double target_vel,
            double current_torq, double target_torq,
            double delta)
        {
            auto velctr  = velocity_control(target_vel - current_vel, delta);
            auto torqctr = torque_control(, delta);
            return torqctr;
        }*/

        /*double position_velocity_torque_control(
            double error_pos, double error_vel, double error_torq, double delta)
        {
            auto posctr  = position_control(error_pos, delta);
            auto velctr  = velocity_control(posctr + error_vel, delta);
            auto torqctr = torque_control(velctr + error_torq, delta);
            return torqctr;
        }*/

        double position_velocity_control(double target_pos,
                                         double current_pos,
                                         double target_vel,
                                         double current_vel,
                                         float alpha,
                                         double delta)
        {
            auto target_vel_by_pos =
                position_control(target_pos, current_pos, delta);
            auto mean_target_vel =
                target_vel_by_pos * (alpha) + target_vel * (1 - alpha);
            auto velctr = velocity_control(mean_target_vel, current_vel, delta);
            return velctr;
        }
    };
}

#endif