#ifndef RALGO_SERVO_SECOND_TIER_H
#define RALGO_SERVO_SECOND_TIER_H

#include <ralgo/filter/aperiodic_filter.h>
#include <ralgo/filter/servo_filter.h>

namespace ralgo
{
    class servo_second_tier : public servo_filter
    {
    public:
        double mark_position = 0;
        double mark_velocity = 0;
        double T = 0.1;

    public:
        servo_second_tier() {}

        void apply_mark_speed(double vel, double delta)
        {
            mark_velocity = vel;
            mark_position += vel * delta;
        }

        double mark_velocity_control(double current_pos,
                                     double target_vel,
                                     double current_vel,
                                     float alpha,
                                     double delta)
        {
            apply_mark_speed(target_vel, delta);
            nos::println("mark_position", mark_position);
            return position_velocity_control(mark_position,
                                             current_pos,
                                             mark_velocity,
                                             current_vel,
                                             alpha,
                                             delta);
        }

        double mark_position_control(double target,
                                     double current_pos,
                                     double current_vel,
                                     float alpha,
                                     double delta)
        {
            double target_vel = (target - mark_position) / T;
            nos::println("target_vel:", target, target_vel);
            return mark_velocity_control(
                current_pos, target_vel, current_vel, alpha, delta);
        }
    };
}

#endif