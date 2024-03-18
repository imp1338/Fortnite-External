#pragma once
#include "Vector3.h"
#include "driver.hh"
#include "sdk.h"
#include "DirectX9/d3dx9.h"
#include "OverlayDefines.h"

#define M_PI 3.14159265358979323846264338327950288419716939937510

namespace ViewPoint 
{
	struct FTransform
	{
		FQuat Rotation; // 0x00(0x10)
		Vector3 Translation; // 0x10(0x0c)
		uint32_t pad0001; // 0x1c(0x04)
		Vector3 Scale3D; // 0x20(0x0c)
		uint32_t pad0002; // 0x2c(0x04)

		FTransform() : Rotation{ (0.f, 0.f, 0.f, 0.f) }, Translation(0.f, 0.f, 0.f), Scale3D(0.f, 0.f, 0.f), pad0001(0), pad0002(0) { }

		FTransform(const FQuat& rot, const Vector3& translation, const Vector3& scale)
		{
			Rotation = rot;
			Translation = translation;
			Scale3D = scale;

			pad0001 = 0;
			pad0002 = 0;
		}

		D3DMATRIX ToMatrixWithScale()
		{
			D3DMATRIX m{};
			m._41 = Translation.x;
			m._42 = Translation.y;
			m._43 = Translation.z;
			float x2 = Rotation.x + Rotation.x;
			float y2 = Rotation.y + Rotation.y;
			float z2 = Rotation.z + Rotation.z;
			float xx2 = Rotation.x * x2;
			float yy2 = Rotation.y * y2;
			float zz2 = Rotation.z * z2;
			m._11 = (1.0f - (yy2 + zz2)) * Scale3D.x;
			m._22 = (1.0f - (xx2 + zz2)) * Scale3D.y;
			m._33 = (1.0f - (xx2 + yy2)) * Scale3D.z;
			float yz2 = Rotation.y * z2;
			float wx2 = Rotation.w * x2;
			m._32 = (yz2 - wx2) * Scale3D.z;
			m._23 = (yz2 + wx2) * Scale3D.y;
			float xy2 = Rotation.x * y2;
			float wz2 = Rotation.w * z2;
			m._21 = (xy2 - wz2) * Scale3D.y;
			m._12 = (xy2 + wz2) * Scale3D.x;
			float xz2 = Rotation.x * z2;
			float wy2 = Rotation.w * y2;
			m._31 = (xz2 + wy2) * Scale3D.z;
			m._13 = (xz2 - wy2) * Scale3D.x;
			m._14 = 0.0f;
			m._24 = 0.0f;
			m._34 = 0.0f;
			m._44 = 1.0f;
			return m;
		}
	};

	D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
	{
		D3DMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}

	D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
	{
		float radpitch = (rot.x * M_PI / 180);
		float radyaw = (rot.y * M_PI / 180);
		float radroll = (rot.z * M_PI / 180);
		float sp = sinf(radpitch);
		float cp = cosf(radpitch);
		float sy = sinf(radyaw);
		float cy = cosf(radyaw);
		float sr = sinf(radroll);
		float cr = cosf(radroll);
		D3DMATRIX matrix{};
		matrix.m[0][0] = cp * cy;
		matrix.m[0][1] = cp * sy;
		matrix.m[0][2] = sp;
		matrix.m[0][3] = 0.f;
		matrix.m[1][0] = sr * sp * cy - cr * sy;
		matrix.m[1][1] = sr * sp * sy + cr * cy;
		matrix.m[1][2] = -sr * cp;
		matrix.m[1][3] = 0.f;
		matrix.m[2][0] = -(cr * sp * cy + sr * sy);
		matrix.m[2][1] = cy * sr - cr * sp * sy;
		matrix.m[2][2] = cr * cp;
		matrix.m[2][3] = 0.f;
		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;
		return matrix;
	}

}


namespace FortViewPoint_Functions {


	bool isVisible(uintptr_t mesh)
	{
		float lst = read<float>(mesh + Offsets::LastSumbitTime);
		float lstos = read<float>(mesh + Offsets::LastRenderTime);
		const float tick = 0.06f;
		return lst + tick >= lstos;
	}

	struct CameraInfo
	{
		Vector3 location;
		Vector3 rotation;
		float fov;
	};

	float fov;

	CameraInfo GetCameraInfo()
	{
		CameraInfo camera;
		auto location_pointer = read<uintptr_t>(Cache::World + 0x110); //110
		auto rotation_pointer = read<uintptr_t>(Cache::World + 0x120); //120

		struct RotationInfo
		{
			double pitch;
			char pad_0008[24];
			double yaw;
			char pad_0028[424];
			double roll;
		} rotationInfo;

		rotationInfo.pitch = read<double>(rotation_pointer);
		rotationInfo.yaw = read<double>(rotation_pointer + 0x20);
		rotationInfo.roll = read<double>(rotation_pointer + 0x1d0);

		camera.location = read<Vector3>(location_pointer);
		camera.rotation.x = asin(rotationInfo.roll) * (180.0 / M_PI);
		camera.rotation.y = ((atan2(rotationInfo.pitch * -1, rotationInfo.yaw) * (180.0 / M_PI)) * -1) * -1;
		camera.fov = read<float>((uintptr_t)Cache::PlayerController + 0x394) * 90.f;

		return camera;
	}

	Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
	{
		CameraInfo viewPoint = GetCameraInfo();
		
		D3DMATRIX temp_matrix = ViewPoint::Matrix(viewPoint.rotation);
		Vector3 vaxisx = Vector3(temp_matrix.m[0][0], temp_matrix.m[0][1], temp_matrix.m[0][2]);
		Vector3 vaxisy = Vector3(temp_matrix.m[1][0], temp_matrix.m[1][1], temp_matrix.m[1][2]);
		Vector3 vaxisz = Vector3(temp_matrix.m[2][0], temp_matrix.m[2][1], temp_matrix.m[2][2]);
		Vector3 vdelta = WorldLocation - viewPoint.rotation;
		Vector3 vtransformed = Vector3(vdelta.Dot(vaxisy), vdelta.Dot(vaxisz), vdelta.Dot(vaxisx));
		if (vtransformed.z < 1) vtransformed.z = 1;
		return Vector3(windef::WinWidth + vtransformed.x * ((windef::WinWidth / tanf(viewPoint.fov * M_PI / 360))) / vtransformed.z, windef::WinHeight - vtransformed.y * ((windef::WinWidth / tanf(viewPoint.fov * M_PI / 360))) / vtransformed.z,0);
	}
};
namespace FortBoneArray_Functions {
	Vector3 GetBoneWithRotation(uintptr_t mesh, int bone_id)
	{
		uintptr_t bone_array = read<uintptr_t>(mesh + 0x598);
		if (bone_array == 0) bone_array = read<uintptr_t>(mesh + 0x620);
		ViewPoint::FTransform bone = read<ViewPoint::FTransform>(bone_array + (bone_id * 0x60));
		ViewPoint::FTransform component_to_world = read<ViewPoint::FTransform>(mesh + 0x1c0);
		D3DMATRIX matrix = ViewPoint::MatrixMultiplication(bone.ToMatrixWithScale(), component_to_world.ToMatrixWithScale());
		return Vector3(matrix._41, matrix._42, matrix._43);
	}
}; 



namespace FortBoneArray_Defines {
	int Root = 0;
	int attach = 1;
	int pelvis = 2;
	int spine_01 = 3;
	int spine_02 = 4;
	int spine_03 = 5;
	int spine_04 = 6;
	int spine_05 = 7;
	int clavicle_l = 8;
	int upperarm_l = 9;
	int lowerarm_l = 10;
	int hand_l = 11;
	int index_metacarpal_l = 12;
	int index_01_l = 13;
	int index_02_l = 14;
	int index_03_l = 15;
	int middle_metacarpal_l = 16;
	int middle_01_l = 17;
	int middle_02_l = 18;
	int middle_03_l = 19;
	int pinky_metacarpal_l = 20;
	int pinky_01_l = 21;
	int pinky_02_l = 22;
	int pinky_03_l = 23;
	int ring_metacarpal_l = 24;
	int ring_01_l = 25;
	int ring_02_l = 26;
	int ring_03_l = 27;
	int thumb_01_l = 28;
	int thumb_02_l = 29;
	int thumb_03_l = 30;
	int weapon_l = 31;
	int hand_attach_l = 32;
	int lowerarm_twist_01_l = 33;
	int lowerarm_twist_02_l = 34;
	int upperarm_twist_01_l = 35;
	int upperarm_twist_02_l = 36;
	int clavicle_r = 37;
	int upperarm_r = 38;
	int lowerarm_r = 39;
	int hand_r = 40;
	int index_metacarpal_r = 41;
	int index_01_r = 42;
	int index_02_r = 43;
	int index_03_r = 44;
	int middle_metacarpal_r = 45;
	int middle_01_r = 46;
	int middle_02_r = 47;
	int middle_03_r = 48;
	int pinky_metacarpal_r = 49;
	int actor_bounds_top = 50;
	int pinky_02_r = 51;
	int pinky_03_r = 52;
	int ring_metacarpal_r = 53;
	int ring_01_r = 54;
	int ring_02_r = 55;
	int ring_03_r = 56;
	int thumb_01_r = 57;
	int thumb_02_r = 58;
	int thumb_03_r = 59;
	int weapon_r = 60;
	int hand_attach_r = 61;
	int lowerarm_twist_01_r = 62;
	int lowerarm_twist_02_r = 63;
	int upperarm_twist_01_r = 64;
	int upperarm_twist_02_r = 65;
	int neck_01 = 66;
	int neck_02 = 67;
	int head = 67;
	int attach_backpack = 69;
	int attach_cape = 70;
	int thigh_l = 71;
	int calf_l = 72;
	int calf_twist_01_l = 73;
	int calf_twist_02_l = 74;
	int foot_l = 75;
	int ball_l = 76;
	int thigh_twist_01_l = 77;
	int thigh_r = 78;
	int calf_r = 79;
	int calf_twist_01_r = 80;
	int calf_twist_02_r = 81;
	int foot_r = 82;
	int ball_r = 83;
	int thigh_twist_01_r = 84;
	int ik_foot_root = 85;
	int ik_foot_l = 86;
	int ik_foot_r = 87;
	int ik_hand_root = 88;
	int ik_hand_gun = 89;
	int ik_hand_l = 90;
	int ik_hand_r = 91;
	int camera_root = 92;
	int Camera = 93;
	int attach_fp = 94;
	int attach_emote_01 = 95;
	int attach_emote_02 = 96;
	int attach_emote_03 = 97;
	int warp_point_01 = 98;
	int warp_point_02 = 99;
	int warp_point_03 = 100;
	int VB_spine_05_weapon_r = 101;
	int VB_VB_spine_05_weapon_r_ik_hand_gun = 102;
	int VB_VB_spine_05_weapon_r_ik_hand_l = 103;
	int VB_spine_05_upperarm_r = 104;
	int VB_VB_spine_05_upperarm_r_lowerarm_r = 105;
	int VB_VB_VB_spine_05_upperarm_r_lowerarm_r_hand_r = 106;
	int VB_ik_foot_root_weapon_l = 107;
	int VB_root_prop = 108;
	int VB_head_fx = 109;
	int VB_Root_hand_l = 110;
	int VB_Root_hand_r = 111;
	int VB_attach_ik_hand_gun = 112;
	int VB_VB_attach_ik_hand_gun_ik_hand_l = 113;
	int VB_VB_attach_ik_hand_gun_ik_hand_r = 114;
	int VB_spine_05_hand_attach_r = 115;
	int VB_VB_spine_05_hand_attach_r_ik_hand_gun = 116;
	int VB_VB_spine_05_hand_attach_r_ik_hand_l = 117;
	int VB_pelvis_hand_attach_r = 118;
	int VB_ik_foot_root_weapon_r = 119;
	int VB_ik_hand_root_ik_hand_l = 120;
	int VB_attach_ik_foot_l = 121;
	int VB_attach_ik_foot_r = 122;
	int VB_pelvis_ik_hand_gun = 123;
	int VB_pelvis_ik_hand_r = 124;
	int VB_pelvis_ik_hand_l = 125;
	int VB_pelvis_attach_emote_01 = 126;
	int VB_pelvis_attach_emote_02 = 127;
	int VB_pelvis_attach_emote_03 = 128;
	int VB_ik_foot_root_hand_attach_l = 129;
	int VB_ik_foot_root_hand_attach_r = 130;
}; 