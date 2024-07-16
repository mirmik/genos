#ifndef RABBIT_GAZEBO_H
#define RABBIT_GAZEBO_H

#include <rabbit/space/htrans3.h>
#include <rabbit/space/screw.h>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

namespace rabbit
{
	enum class PoseMode
	{
		World = 0,
		CoG = 1,
		Inertial = 2
	};

	static htrans3<double> Pose3d_to_htrans3(ignition::math::Pose3d Pose)
	{
		htrans3<double> ret;

		auto c = Pose.Pos();
		auto r = Pose.Rot();

		ret.lin = { c.X(), c.Y(), c.Z() };
		ret.ang = { r.X(), r.Y(), r.Z(), r.W() };

		return ret;
	}

	static linalg::vec<double, 3> Vector3d_to_vec3(ignition::math::Vector3d V)
	{
		return { V.X(), V.Y(), V.Z() };
	}

	static htrans3<double> gazebo_link_pose(
	    gazebo::physics::LinkPtr link, PoseMode mode = PoseMode::CoG)
	{
		ignition::math::Pose3d pos;

		switch (mode)
		{
			case PoseMode::World: pos = link->WorldPose(); break;
			case PoseMode::CoG: pos = link->WorldCoGPose(); break;
			case PoseMode::Inertial: pos = link->WorldInertialPose(); break;
		}

		return Pose3d_to_htrans3(pos);
	}

	static screw<double,3> gazebo_link_speed(
	    gazebo::physics::LinkPtr link)
	{
		auto w = Vector3d_to_vec3(link->WorldAngularVel());
		auto v = Vector3d_to_vec3(link->WorldCoGLinearVel());

		return {w,v};
	}

	static htrans3<double> gazebo_link_initial_relative_pose(
	    gazebo::physics::LinkPtr link)
	{
		ignition::math::Pose3d pos = link->InitialRelativePose();
		return Pose3d_to_htrans3(pos);
	}

	static htrans3<double> gazebo_link_relative_pose(
	    gazebo::physics::LinkPtr link)
	{
		ignition::math::Pose3d pos = link->RelativePose();
		return Pose3d_to_htrans3(pos);
	}

	static htrans3<double> gazebo_joint_anchor_pose(
	    gazebo::physics::JointPtr joint)
	{
		return Pose3d_to_htrans3(joint->InitialAnchorPose());
	}

	static screw<double, 3> gazebo_joint_local_axis(
	    gazebo::physics::JointPtr joint)
	{
		return
		{
			Vector3d_to_vec3(joint->LocalAxis(0)),
			linalg::vec<double, 3>{}
		};
	}

	static screw<double, 3> gazebo_joint_reaction(
	    gazebo::physics::JointPtr joint)
	{
		auto wrench = joint->GetForceTorque(0);

		return
		{
			Vector3d_to_vec3(wrench.body1Torque),
			Vector3d_to_vec3(wrench.body1Force) };
	}
}

#endif