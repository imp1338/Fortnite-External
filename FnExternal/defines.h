#pragma once
#include <iostream>
#include <string>
#include "string_enc.hpp"
#include "func_enc.h"
#include "Vector3.h"

typedef struct
{
	DWORD R;
	DWORD G;
	DWORD B;
	DWORD A;
}RGBA;
class Color
{
public:
	RGBA red = { 255,0,0,255 };
	RGBA redlol = { 255,0,0,200 };
	RGBA Magenta = { 255,0,255,255 };
	RGBA yellow = { 255,255,0,255 };
	RGBA grayblue = { 128,128,255,255 };
	RGBA green = { 128,224,0,255 };
	RGBA darkgreen = { 0,224,128,255 };
	RGBA brown = { 192,96,0,255 };
	RGBA pink = { 255,168,255,255 };
	RGBA DarkYellow = { 216,216,0,255 };
	RGBA SilverWhite = { 236,236,236,255 };
	RGBA purple = { 144,0,255,255 };
	RGBA Navy = { 88,48,224,255 };
	RGBA skyblue = { 0,136,255,255 };
	RGBA graygreen = { 128,160,128,255 };
	RGBA blue = { 0,96,192,255 };
	RGBA orange = { 255,128,0,255 };
	RGBA peachred = { 255,80,128,255 };
	RGBA reds = { 255,128,192,255 };
	RGBA darkgray = { 96,96,96,255 };
	RGBA Navys = { 0,0,128,255 };
	RGBA cyan = { 0,255,255,255 };
	RGBA darkgreens = { 0,128,0,255 };
	RGBA darkblue = { 0,128,128,255 };
	RGBA redbrown = { 128,0,0,255 };
	RGBA purplered = { 128,0,128,255 };
	RGBA greens = { 0,255,0,255 };
	RGBA envy = { 0,255,255,255 };
	RGBA black = { 100,0,0,0 };
	RGBA gray = { 128,128,128,255 };
	RGBA white = { 255,255,255,255 };
	RGBA blues = { 30,144,255,255 };
	RGBA lightblue = { 135,206,250,160 };
	RGBA Scarlet = { 220, 20, 60, 160 };
	RGBA white_ = { 255,255,255,200 };
	RGBA gray_ = { 128,128,128,200 };
	RGBA black_ = { 100,000, 000, 000 };
	RGBA red_ = { 255,0,0,200 };
	RGBA Magenta_ = { 255,0,255,200 };
	RGBA yellow_ = { 255,255,0,200 };
	RGBA grayblue_ = { 128,128,255,200 };
	RGBA green_ = { 128,224,0,200 };
	RGBA darkgreen_ = { 0,224,128,200 };
	RGBA brown_ = { 192,96,0,200 };
	RGBA pink_ = { 255,168,255,200 };
	RGBA darkyellow_ = { 216,216,0,200 };
	RGBA silverwhite_ = { 236,236,236,200 };
	RGBA purple_ = { 144,0,255,200 };
	RGBA Blue_ = { 88,48,224,200 };
	RGBA skyblue_ = { 0,136,255,200 };
	RGBA graygreen_ = { 128,160,128,200 };
	RGBA blue_ = { 0,96,192,200 };
	RGBA orange_ = { 255,128,0,200 };
	RGBA pinks_ = { 255,80,128,200 };
	RGBA Fuhong_ = { 255,128,192,200 };
	RGBA darkgray_ = { 96,96,96,200 };
	RGBA Navy_ = { 0,0,128,200 };
	RGBA darkgreens_ = { 0,128,0,200 };
	RGBA darkblue_ = { 0,128,128,200 };
	RGBA redbrown_ = { 128,0,0,200 };
	RGBA purplered_ = { 128,0,128,200 };
	RGBA greens_ = { 0,255,0,200 };
	RGBA envy_ = { 0,255,255,200 };

	RGBA glassblack = { 0, 0, 0, 160 };
	RGBA GlassBlue = { 65,105,225,80 };
	RGBA glassyellow = { 255,255,0,160 };
	RGBA glass = { 200,200,200,60 };

	RGBA filled = { 0, 0, 0, 150 };

	RGBA Plum = { 221,160,221,160 };

	RGBA VisibleColor = { 255 / 255, 0, 0 };
	RGBA InvisibleColor = { 0, 255 / 255, 0 };
	RGBA FilledFov = { 0,0,0,160 };

	RGBA rainbow() {

		static float x = 0, y = 0;
		static float r = 0, g = 0, b = 0;

		if (y >= 0.0f && y < 255.0f) {
			r = 255.0f;
			g = 0.0f;
			b = x;
		}
		else if (y >= 255.0f && y < 510.0f) {
			r = 255.0f - x;
			g = 0.0f;
			b = 255.0f;
		}
		else if (y >= 510.0f && y < 765.0f) {
			r = 0.0f;
			g = x;
			b = 255.0f;
		}
		else if (y >= 765.0f && y < 1020.0f) {
			r = 0.0f;
			g = 255.0f;
			b = 255.0f - x;
		}
		else if (y >= 1020.0f && y < 1275.0f) {
			r = x;
			g = 255.0f;
			b = 0.0f;
		}
		else if (y >= 1275.0f && y < 1530.0f) {
			r = 255.0f;
			g = 255.0f - x;
			b = 0.0f;
		}

		float rainbow_speed = 0.25f;

		x += rainbow_speed; //increase this value to switch colors faster
		if (x >= 255.0f)
			x = 0.0f;

		y += rainbow_speed; //increase this value to switch colors faster
		if (y > 1530.0f)
			y = 0.0f;


		return RGBA{ (DWORD)r, (DWORD)g, (DWORD)b, 255 };
	}


};
static Color Col;


void DrawBox(int X, int Y, int W, int H, const ImU32& color, int thickness, int borderPx) {
	float lineW = (W / 1);
	float lineH = (H / 1);


	//corners
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void DrawSkeleton(uintptr_t CurrentActorMesh, ImColor color, float thickness)
{
	Vector3 vHeadBone = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 67);
	Vector3 vHip = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 3);
	Vector3 vNeck = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 66);
	Vector3 vUpperArmLeft = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 9);
	Vector3 vUpperArmRight = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 38);
	Vector3 vElbowLeft = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 10);
	Vector3 vElbowRight = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 39);
	Vector3 vLeftHand = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 33);
	Vector3 vRightHand = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 62);
	Vector3 vLeftHand1 = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 33);
	Vector3 vRightHand1 = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 62);
	Vector3 vRightThigh = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 78);
	Vector3 vLeftThigh = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 71);
	Vector3 vRightCalf = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 79); 
	Vector3 vLeftCalf = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 72);
	Vector3 vLeftFoot = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 73);
	Vector3 vRightFoot = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 80);
	Vector3 vLeftHeel = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 75);
	Vector3 vRightHeel = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 82);
	Vector3 vLeftToe = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 76);
	Vector3 vRightToe = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 83);


	Vector3 vHeadBoneOut = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z));
	Vector3 vHipOut = FortViewPoint_Functions::ProjectWorldToScreen(vHip);
	Vector3 vNeckOut = FortViewPoint_Functions::ProjectWorldToScreen(vNeck);
	Vector3 vUpperArmLeftOut = FortViewPoint_Functions::ProjectWorldToScreen(vUpperArmLeft);
	Vector3 vUpperArmRightOut = FortViewPoint_Functions::ProjectWorldToScreen(vUpperArmRight);
	Vector3 vElbowLeftOut = FortViewPoint_Functions::ProjectWorldToScreen(vElbowLeft);
	Vector3 vElbowRightOut = FortViewPoint_Functions::ProjectWorldToScreen(vElbowRight);
	Vector3 vLeftHandOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftHand);
	Vector3 vRightHandOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightHand);
	Vector3 vLeftHandOut1 = FortViewPoint_Functions::ProjectWorldToScreen(vLeftHand1);
	Vector3 vRightHandOut1 = FortViewPoint_Functions::ProjectWorldToScreen(vRightHand1);
	Vector3 vLeftHeelOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftHeel);
	Vector3 vRightHeelOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightHeel);
	Vector3 vRightThighOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightThigh);
	Vector3 vLeftThighOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftThigh);
	Vector3 vRightCalfOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightCalf);
	Vector3 vLeftCalfOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftCalf);
	Vector3 vLeftFootOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftFoot);
	Vector3 vRightFootOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightFoot);
	Vector3 vLeftToeOut = FortViewPoint_Functions::ProjectWorldToScreen(vLeftToe);
	Vector3 vRightToeOut = FortViewPoint_Functions::ProjectWorldToScreen(vRightToe);


	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vHeadBoneOut.x, vHeadBoneOut.y), ImVec2(vNeckOut.x, vNeckOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vHipOut.x, vHipOut.y), ImVec2(vNeckOut.x, vNeckOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), ImVec2(vNeckOut.x, vNeckOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), ImVec2(vNeckOut.x, vNeckOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vElbowLeftOut.x, vElbowLeftOut.y), ImVec2(vUpperArmLeftOut.x, vUpperArmLeftOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vElbowRightOut.x, vElbowRightOut.y), ImVec2(vUpperArmRightOut.x, vUpperArmRightOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftHandOut.x, vLeftHandOut.y), ImVec2(vElbowLeftOut.x, vElbowLeftOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightHandOut.x, vRightHandOut.y), ImVec2(vElbowRightOut.x, vElbowRightOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftHandOut.x, vLeftHandOut.y), ImVec2(vLeftHandOut1.x, vLeftHandOut1.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightHandOut.x, vRightHandOut.y), ImVec2(vRightHandOut1.x, vRightHandOut1.y), color, thickness);

	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftThighOut.x, vLeftThighOut.y), ImVec2(vHipOut.x, vHipOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightThighOut.x, vRightThighOut.y), ImVec2(vHipOut.x, vHipOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), ImVec2(vLeftThighOut.x, vLeftThighOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightCalfOut.x, vRightCalfOut.y), ImVec2(vRightThighOut.x, vRightThighOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftFootOut.x, vLeftFootOut.y), ImVec2(vLeftCalfOut.x, vLeftCalfOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightFootOut.x, vRightFootOut.y), ImVec2(vRightCalfOut.x, vRightCalfOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftFootOut.x, vLeftFootOut.y), ImVec2(vLeftHeelOut.x, vLeftHeelOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightFootOut.x, vRightFootOut.y), ImVec2(vRightHeelOut.x, vRightHeelOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vLeftHeelOut.x, vLeftHeelOut.y), ImVec2(vLeftToeOut.x, vLeftToeOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vRightHeelOut.x, vRightHeelOut.y), ImVec2(vRightToeOut.x, vRightToeOut.y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(vNeckOut.x, vNeckOut.y), ImVec2(vHeadBoneOut.x, vHeadBoneOut.y), color, thickness);
	return;
}

Vector3 p;

void SimpleBox(int x, int y, ImColor color, const char* str, const char* key, bool v)
{
	ImFont a;
	std::string utf_8_2 = str;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 15, p.y + y + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());

	std::string utf_8_22 = key;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 - 5, p.y + y + 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 - 5, p.y + y - 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 - 5, p.y + y - 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 - 5, p.y + y + 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 5, p.y + y - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_22.c_str());

	if (v) {
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "ON");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "ON");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "ON");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "ON");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 160, p.y + y - 6), ImGui::ColorConvertFloat4ToU32(ImColor(0, 255, 0, 255)), "ON");

	}
	if (!v) {
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "OFF");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "OFF");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "OFF");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), "OFF");
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 160, p.y + y - 6), ImGui::ColorConvertFloat4ToU32(ImColor(255, 0, 0, 255)), "OFF");
	}


	//Draw::BackgroundFilledRBG(p.x + x, p.y + y, 15, 10, ImColor(0, 255, 150, 255))

}

void SimpleText(int x, int y, ImColor color, const char* str)
{
	ImFont a;
	std::string utf_8_2 = str;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 15, p.y + y + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
}

void SimpleBoxValue(int x, int y, ImColor color, const char* str, const char* key, int v)
{
	ImFont a;
	std::string utf_8_2 = str;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 15, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 15, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_2.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 15, p.y + y + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());

	std::string utf_8_22 = key;
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 - 5, p.y + y + 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 - 5, p.y + y - 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 - 5, p.y + y - 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 - 5, p.y + y + 1 - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), utf_8_22.c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 5, p.y + y - 6), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_22.c_str());

	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), std::to_string(v).c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), std::to_string(v).c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 1 + 160, p.y + y - 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), std::to_string(v).c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x - 1 + 160, p.y + y + 1 + -6), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), std::to_string(v).c_str());
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(p.x + x + 160, p.y + y - 6), ImGui::ColorConvertFloat4ToU32(ImColor(255, 0, 0, 255)), std::to_string(v).c_str());
	//Draw::BackgroundFilledRBG(p.x + x, p.y + y, 15, 10, ImColor(0, 255, 150, 255))

}

void DrawCorneredBox(int x, int y, int w, int h, ImColor color, int thickness) {
	float line_w = (w / 3);
	float line_h = (h / 3);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + line_h), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + line_w, y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w - line_w, y), ImVec2(x + w, y), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + line_h), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y + h - line_h), ImVec2(x, y + h), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + line_w, y + h), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w - line_w, y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x + w, y + h - line_h), ImVec2(x + w, y + h), color, thickness);
}

VOID EraseMemory()
{
	DWORD oldProtect = 0;
	char* baseAddress = (char*)GetModuleHandle(NULL);

	// Change the protection of the memory region to allow writing
	if (!VirtualProtect(baseAddress, 4096, PAGE_READWRITE, &oldProtect)) {
	}

	// Zero out the memory region
	SecureZeroMemory(baseAddress, 4096);

	// Restore the original memory protection
	DWORD restoredProtect;
	if (!VirtualProtect(baseAddress, 4096, oldProtect, &restoredProtect)) {
	}
}

template<std::intmax_t FPS>
class frame_rater {
public:
	frame_rater() :                 // initialize the object keeping the pace
		time_between_frames{ 1 },     // std::ratio<1, FPS> seconds
		tp{ std::chrono::steady_clock::now() }
	{}

	void sleep() {
		// add to time point
		tp += time_between_frames;

		// and sleep until that time point
		std::this_thread::sleep_until(tp);
	}

private:
	// a duration with a length of 1/FPS seconds
	std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames;

	// the time point we'll add to in every loop
	std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};

std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}


std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

void DrawString(float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[1024] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), color, text.c_str());
}