#ifndef RALGO_ROBO_POSVEL_INTERFACES_H
#define RALGO_ROBO_POSVEL_INTERFACES_H

namespace robo
{
    class i_torque_setter
    {
    public:
        virtual void set_torque(double val) = 0;
        virtual ~i_torque_setter() = default;
    };

    class i_velocity_setter
    {
    public:
        virtual void set_velocity(double vel) = 0;
        virtual ~i_velocity_setter() = default;
    };

    class i_position_setter
    {
    public:
        virtual void set_position(double vel) = 0;
        virtual ~i_position_setter() = default;
    };

    class i_velocity_feedback
    {
    public:
        virtual double feedback_velocity() = 0;
        virtual ~i_velocity_feedback() = default;
    };

    class i_position_feedback
    {
    public:
        virtual double feedback_position() = 0;
        virtual ~i_position_feedback() = default;
    };

    class i_velocity_driver : public i_velocity_setter,
                              public i_velocity_feedback
    {
    };

    class i_position_driver : public i_position_setter,
                              public i_position_feedback
    {
    };
}

#endif
