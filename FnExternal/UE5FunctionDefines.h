#pragma once
#include <cstdint>
#include "driver.hh"
#include "HxDef.hh"
#include "ImGui/imgui.h"

#pragma warning(disable : 4996)

namespace UE5 {


	class ExtraDrawings {

	private:
		Vector3 RotatePoint(Vector3 EntityPos, Vector3 LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck, bool angleInRadians)
		{
			float r_1, r_2;
			float x_1, y_1;

			r_1 = EntityPos.y - LocalPlayerPos.y;
			r_2 = EntityPos.x - LocalPlayerPos.x;

			float Yaw = angle - 90.0f;

			float yawToRadian = angleInRadians ? Yaw : Yaw * static_cast<float>(M_PI / 180.0F);
			x_1 = (r_2 * cos(yawToRadian) - r_1 * sin(yawToRadian)) / 20;
			y_1 = (r_2 * sin(yawToRadian) + r_1 * cos(yawToRadian)) / 20;

			*viewCheck = y_1 < 0;

			x_1 *= zoom;
			y_1 *= zoom;

			int sizX = sizeX / 2;
			int sizY = sizeY / 2;

			x_1 += sizX;
			y_1 += sizY;

			if (x_1 < 5)
				x_1 = 5;

			if (x_1 > sizeX - 5)
				x_1 = sizeX - 5;

			if (y_1 < 5)
				y_1 = 5;

			if (y_1 > sizeY - 5)
				y_1 = sizeY - 5;

			x_1 += posX;
			y_1 += posY;

			return Vector3(x_1, y_1, 0);
		}

	public:
		void DrawThreeDimensionalBox(DWORD_PTR CurrentActorMesh, int bone_botton, int bone_top, float offset, ImU32 Color)
		{
			Vector3 vRootBone = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, bone_botton);
			Vector3 vHeadBone = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, bone_top);
			Vector3 vHeadBoneOut = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z + offset));

			if (vHeadBoneOut.x != 0 || vHeadBoneOut.y != 0 || vHeadBoneOut.z != 0)
			{
				ImU32 ESPSkeleton = ImColor(0, 255, 0, 255);

				Vector3 bottom1 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vRootBone.x + 10, vRootBone.y - 10, vRootBone.z));
				Vector3 bottom2 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vRootBone.x - 10, vRootBone.y - 10, vRootBone.z));
				Vector3 bottom3 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vRootBone.x - 10, vRootBone.y + 10, vRootBone.z));
				Vector3 bottom4 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vRootBone.x + 10, vRootBone.y + 10, vRootBone.z));

				Vector3 top1 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x + 10, vHeadBone.y - 10, vHeadBone.z + 10));
				Vector3 top2 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x - 10, vHeadBone.y - 10, vHeadBone.z + 10));
				Vector3 top3 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x - 10, vHeadBone.y + 10, vHeadBone.z + 10));
				Vector3 top4 = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x + 10, vHeadBone.y + 10, vHeadBone.z + 10));

				auto& drawList = *ImGui::GetBackgroundDrawList();

				drawList.AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ESPSkeleton, 0.1f);

				drawList.AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), ESPSkeleton, 0.1f);

				drawList.AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ESPSkeleton, 0.1f);
				drawList.AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ESPSkeleton, 0.1f);
			}
		}
	};
	class MOUClass_EX {
	public:
		void SmoothMouseMovement(float x, float y)
		{
		    float ScreenCenterX = windef::ScreenCenterX;
		    float ScreenCenterY = windef::ScreenCenterY;
			const int AimSpeed = FortOptions::Smoothing;

			float TargetX = 0;
			float TargetY = 0;

			// Calculate movement along the X-axis
			if (x != 0)
			{
				if (x > ScreenCenterX)
				{
					TargetX = (x - ScreenCenterX) / AimSpeed;
					if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
				}
				else if (x < ScreenCenterX)
				{
					TargetX = -(ScreenCenterX - x) / AimSpeed;
					if (TargetX + ScreenCenterX < 0) TargetX = 0;
				}
			}

			// Calculate movement along the Y-axis
			if (y != 0)
			{
				if (y > ScreenCenterY)
				{
					TargetY = (y - ScreenCenterY) / AimSpeed;
					if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
				}
				else if (y < ScreenCenterY)
				{
					TargetY = -(ScreenCenterY - y) / AimSpeed;
					if (TargetY + ScreenCenterY < 0) TargetY = 0;
				}
			}

			// Move the mouse
			Imported::Move_Mouse(static_cast<int>(TargetX), static_cast<int>(TargetY));
		}
	};
	class PlayerState
	{
	public :
		__forceinline static std::string ReadGetNameFromFName(int key)
		{
			uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
			uint16_t NameOffset = (uint16_t)key;

			uint64_t NamePoolChunk = read<uint64_t>(Cache::BaseAddress + 0x114BF280 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);
			uint16_t nameEntry = read<uint16_t>(NamePoolChunk);

			int nameLength = nameEntry >> 6;
			char buff[1024];
			if ((uint32_t)nameLength) {
				for (int x = 0; x < nameLength; ++x) {
					buff[x] = read<char>(NamePoolChunk + 4 + x);
				}

				char* v2 = buff;
				int v5 = 0;
				int v4 = nameLength;
				DWORD dword_113C6B04 = read<DWORD>(Cache::BaseAddress + 0x113C6B04);
				int v6 = dword_113C6B04 ^ 0x9C677CC5;
				__int64 result = 33 * (dword_113C6B04 / 0x21u);
				__int64 v8 = dword_113C6B04 % 0x21u;
				if (v4)
				{
					do
					{
						result = v5 + v8;
						++v5;
						v6 += result;
						*v2++ ^= v6;
					} while (v5 < v4);
				}

				buff[nameLength] = '\0';
				return std::string(buff);
			}
			else {
				return "error";
			}
		}

		inline std::string find_object_name(INT32 object_id) {
			uint32_t ChunkOffset = (uint32_t)((int)(object_id) >> 16);
			uint16_t NameOffset = (uint16_t)object_id;

			uint64_t NamePoolChunk = read<uint64_t>(Cache::BaseAddress + 0x114BF280 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset);
			if (read<uint16_t>(NamePoolChunk) < 64)
			{
				auto a1 = read<DWORD>(NamePoolChunk + 4);
				return ReadGetNameFromFName(a1);
			}
			else
			{
				return ReadGetNameFromFName(object_id);
			}
		}

		__forceinline auto ReturnPlatform(uintptr_t PlayerState) -> std::string
		{
			uintptr_t test_platform = read<uintptr_t>(PlayerState + 0x438);
			wchar_t platform[64] = { 0 }; // Initialize the array to avoid potential issues
			MemoryModule->ReadProcessMemory(reinterpret_cast<PVOID>(test_platform), reinterpret_cast<uint8_t*>(platform), sizeof(platform));
			std::wstring platform_wstr(platform);
			std::string platform_str(platform_wstr.begin(), platform_wstr.end());
			return platform_str;
		}

		std::string username(std::uintptr_t playerstate)
		{
			int pNameLength = 0;
			char16_t* pNameBuffer = nullptr;

			uintptr_t pNameStructure = read<uintptr_t>(playerstate + 0xAE0);
			pNameLength = read<int>(pNameStructure + 0x4e);
			if (pNameLength <= 0)
				return "AI / BOT";

			pNameBuffer = new char16_t[pNameLength];
			uintptr_t pNameEncryptedBuffer = read<uintptr_t>(pNameStructure + 0x8);
			//(pNameEncryptedBuffer, reinterpret_cast<uint8_t*>(pNameBuffer), pNameLength * sizeof(char16_t));

			for (int i = 0; i < pNameLength; ++i)
			{
				pNameBuffer[i] ^= (i * 33 + (read<DWORD>(Cache::BaseAddress + 0x113C6B04) ^ 0x9C677CC5));
			}

			std::u16string temp_wstring(pNameBuffer, pNameLength);
			delete[] pNameBuffer;

			return std::string(temp_wstring.begin(), temp_wstring.end());
		}
	};
}


UE5::PlayerState* APlayerState = new UE5::PlayerState();
UE5::MOUClass_EX* MOUClass = new UE5::MOUClass_EX();
UE5::ExtraDrawings* EDrawings = new UE5::ExtraDrawings();