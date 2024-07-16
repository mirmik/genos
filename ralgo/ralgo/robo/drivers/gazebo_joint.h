#ifndef RALGO_ROBO_DRIVERS_GAZEBO_JOINT_H
#define RALGO_ROBO_DRIVERS_GAZEBO_JOINT_H

#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>
#include <ralgo/robo/iposvel.h>

namespace robo
{
    class gazebo_joint : public i_velocity_feedback,
                         public i_position_feedback,
                         public i_torque_setter
    {
        gazebo::physics::JointPtr _joint;

    public:
        gazebo_joint() = default;
        gazebo_joint(gazebo::physics::JointPtr joint)
        {
            bind(joint);
        }

        double feedback_position() override
        {
            return _joint->Position(0);
        }

        double feedback_velocity() override
        {
            return _joint->GetVelocity(0);
        }

        void set_torque(double val) override
        {
            _joint->SetForce(0, val);
        }

        void bind(gazebo::physics::JointPtr joint)
        {
            _joint = joint;
            _joint->SetProvideFeedback(true);
        }
    };
}

#endif
