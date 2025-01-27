#include "pch_script.h"

#include "physics_shell_scripted.h"

#include "physics_element_scripted.h"
#include "physics_joint_scripted.h"

using namespace luabind;


cphysics_element_scripted* cphysics_shell_scripted::get_Element(LPCSTR bone_name)
{
	CPhysicsElement* E = physics_impl().get_Element(bone_name);
	if (!E)
		return 0;
	return get_script_wrapper<cphysics_element_scripted>(*E);
}

cphysics_element_scripted* cphysics_shell_scripted::get_Element(u16 bone_id)
{
	CPhysicsElement* E = physics_impl().get_Element(bone_id);
	if (!E)
		return 0;
	return get_script_wrapper<cphysics_element_scripted>(*E);
}

cphysics_element_scripted* cphysics_shell_scripted::get_ElementByStoreOrder(u16 idx)
{
	CPhysicsElement* E = physics_impl().get_ElementByStoreOrder(idx);
	R_ASSERT(E);
	return get_script_wrapper<cphysics_element_scripted>(*E);
}


cphysics_joint_scripted* cphysics_shell_scripted::get_Joint(LPCSTR bone_name)
{
	CPhysicsJoint* J = physics_impl().get_Joint(bone_name);
	if (!J)
		return 0;
	return get_script_wrapper<cphysics_joint_scripted>(*J);
}

cphysics_joint_scripted* cphysics_shell_scripted::get_Joint(u16 bone_id)
{
	CPhysicsJoint* J = physics_impl().get_Joint(bone_id);
	if (!J)
		return 0;
	return get_script_wrapper<cphysics_joint_scripted>(*J);
}

cphysics_joint_scripted* cphysics_shell_scripted::get_JointByStoreOrder(u16 idx)
{
	CPhysicsJoint* J = physics_impl().get_JointByStoreOrder(idx);
	R_ASSERT(J);
	return get_script_wrapper<cphysics_joint_scripted>(*J);
}

void cphysics_shell_scripted::freeze()
{
	u16 max_elements = get_ElementsNumber();
	for (u16 i = 0; i < max_elements; ++i)
	{
		get_ElementByStoreOrder(i)->Fix();
	}
	physics_impl().collide_class_bits().set(2, TRUE); //cbNCStatic
}

void cphysics_shell_scripted::unfreeze()
{
	u16 max_elements = get_ElementsNumber();
	for (u16 i = 0; i < max_elements; ++i)
	{
		get_ElementByStoreOrder(i)->ReleaseFixed();
	}
	physics_impl().collide_class_bits().set(2, FALSE); //cbNCStatic
}

#pragma optimize("s",on)
void cphysics_shell_scripted::script_register(lua_State* L)
{
	module(L)
	[
		class_<cphysics_shell_scripted>("physics_shell")
		.def("apply_force",
		     (void (cphysics_shell_scripted::*)(float, float, float))(&cphysics_shell_scripted::applyForce))

		// demonized: apply_torque
		.def("apply_torque",
			(void (cphysics_shell_scripted::*)(float, float, float))(&cphysics_shell_scripted::applyTorque))

		.def("get_element_by_bone_name",
		     (cphysics_element_scripted*(cphysics_shell_scripted::*)(LPCSTR))(&cphysics_shell_scripted::get_Element))
		.def("get_element_by_bone_id",
		     (cphysics_element_scripted*(cphysics_shell_scripted::*)(u16))(&cphysics_shell_scripted::get_Element))
		.def("get_element_by_order",
		     (cphysics_element_scripted*(cphysics_shell_scripted::*)(u16))(&cphysics_shell_scripted::
			     get_ElementByStoreOrder))
		.def("get_elements_number", &cphysics_shell_scripted::get_ElementsNumber)
		.def("get_joint_by_bone_name",
		     (cphysics_joint_scripted*(cphysics_shell_scripted::*)(LPCSTR))(&cphysics_shell_scripted::get_Joint))
		.def("get_joint_by_bone_id",
		     (cphysics_joint_scripted*(cphysics_shell_scripted::*)(u16))(&cphysics_shell_scripted::get_Joint))
		.def("get_joint_by_order", &cphysics_shell_scripted::get_JointByStoreOrder)
		.def("get_joints_number", &cphysics_shell_scripted::get_JointsNumber)
		.def("block_breaking", &cphysics_shell_scripted::BlockBreaking)
		.def("unblock_breaking", &cphysics_shell_scripted::UnblockBreaking)
		.def("is_breaking_blocked", &cphysics_shell_scripted::IsBreakingBlocked)
		.def("is_breakable", &cphysics_shell_scripted::isBreakable)
		.def("get_linear_vel", &cphysics_shell_scripted::get_LinearVel)
		.def("get_angular_vel", &cphysics_shell_scripted::get_AngularVel)
		.def("freeze", &cphysics_shell_scripted::freeze)
		.def("unfreeze", &cphysics_shell_scripted::unfreeze)
	];
}

/*
void CPhysicsJoint::script_register(lua_State *L)
{
	module(L)
		[
			class_<CPhysicsJoint>("physics_joint")
			.def("get_bone_id",							&CPhysicsJoint::BoneID)
			.def("get_first_element",					&CPhysicsJoint::PFirst_element)
			.def("get_stcond_element",					&CPhysicsJoint::PSecond_element)
			.def("set_anchor_global",					(void(CPhysicsJoint::*)(const float,const float,const float))(&CPhysicsJoint::SetAnchor))
			.def("set_anchor_vs_first_element",			(void(CPhysicsJoint::*)(const float,const float,const float))(&CPhysicsJoint::SetAnchorVsFirstElement))
			.def("set_anchor_vs_second_element",		(void(CPhysicsJoint::*)(const float,const float,const float))(&CPhysicsJoint::SetAnchorVsSecondElement))
			.def("get_axes_number",						&CPhysicsJoint::GetAxesNumber)
			.def("set_axis_spring_dumping_factors",		&CPhysicsJoint::SetAxisSDfactors)
			.def("set_joint_spring_dumping_factors",	&CPhysicsJoint::SetJointSDfactors)
			.def("set_axis_dir_global",					(void(CPhysicsJoint::*)(const float,const float,const float,const int ))(&CPhysicsJoint::SetAxisDir))
			.def("set_axis_dir_vs_first_element",		(void(CPhysicsJoint::*)(const float,const float,const float,const int ))(&CPhysicsJoint::SetAxisDirVsFirstElement))
			.def("set_axis_dir_vs_second_element",		(void(CPhysicsJoint::*)(const float,const float,const float,const int ))(&CPhysicsJoint::SetAxisDirVsSecondElement))
			.def("set_limits",							&CPhysicsJoint::SetLimits)
			.def("set_max_force_and_velocity",			&CPhysicsJoint::SetForceAndVelocity)
			.def("get_max_force_and_velocity",			&CPhysicsJoint::GetMaxForceAndVelocity)
			.def("get_axis_angle",						&CPhysicsJoint::GetAxisAngle)
			.def("get_limits",							&CPhysicsJoint::GetLimits,out_value(_2) + out_value(_3))
			.def("get_axis_dir",						&CPhysicsJoint::GetAxisDirDynamic)
			.def("get_anchor",							&CPhysicsJoint::GetAnchorDynamic)
			.def("is_breakable",						&CPhysicsJoint::isBreakable)
		];
}*/
