#include <rabbit/space/gazebo.h>

namespace gazebo
{
	htrans3<double> Pose3d_to_htrans3(ignition::math::Pose3d Pose)
	{
		htrans3<double> ret;

		auto c = Pose.Pos();
		auto r = Pose.Rot();

		ret.lin = { c.X(), c.Y(), c.Z() };
		ret.ang = { r.X(), r.Y(), r.Z(), r.W() };

		return ret;
	}

	linalg::vec<double, 3> Vector3d_to_vec3(ignition::math::Vector3d V)
	{
		return { V.X(), V.Y(), V.Z() };
	}

	htrans3<double> gazebo_link_pose(
	    gazebo::physics::LinkPtr link, PoseMode mode)
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

	htrans3<double> gazebo_joint_anchor_pose(
	    gazebo::physics::JointPtr joint)
	{
		return Pose3d_to_htrans3(joint->InitialAnchorPose());
	}

	screw<double, 3> gazebo_joint_local_axis(
	    gazebo::physics::JointPtr joint)
	{
		return
		{
			Vector3d_to_vec3(joint->LocalAxis(0)),
			linalg::vec<double, 3>{}
		};
	}

	screw<double, 3> gazebo_joint_reaction(
	    gazebo::physics::JointPtr joint)
	{
		auto wrench = joint->GetForceTorque(0);

		return
		{
			Vector3d_to_vec3(wrench.body1Torque),
			Vector3d_to_vec3(wrench.body1Force) };
	}
}