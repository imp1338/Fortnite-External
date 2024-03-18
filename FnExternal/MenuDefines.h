#pragma once
#include "ImGui/imgui.h"
#include <string>
#include <stringapiset.h>

namespace MenuDefines {
	class MenuDefines {
	public:
		std::string string_To_UTF8(const std::string& str)
		{
			int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t* pwBuf = new wchar_t[nwLen + 1];
			ZeroMemory(pwBuf, nwLen * 2 + 2);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
			int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
			char* pBuf = new char[nLen + 1];
			ZeroMemory(pBuf, nLen + 1);
			WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
			std::string retStr(pBuf);
			delete[]pwBuf;
			delete[]pBuf;
			pwBuf = NULL;
			pBuf = NULL;
			return retStr;
		}
		void BackgroundFilledRBG(int x, int y, int w, int h, ImColor color)
		{
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0);
		}
		void OutlinedRBGText(int x, int y, ImColor color, const char* str)
		{
			ImFont a;
			std::string utf_8_1 = std::string(str);
			std::string utf_8_2 = string_To_UTF8(utf_8_1);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 30 / 30.0)), utf_8_2.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 30 / 30.0)), utf_8_2.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
		}
		void BackgroundRBGOutline(int x, int y, int w, int h, ImColor color, int thickness)
		{
			ImGui::GetBackgroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), 0, 0, thickness);
		}
		void BackgroundFilled(int x, int y, int w, int h, ImColor color)
		{
			ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0);
		}
		void RegularText(int x, int y, ImColor color, const char* str)
		{
			ImFont a;
			std::string utf_8_1 = std::string(str);
			std::string utf_8_2 = string_To_UTF8(utf_8_1);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), color, utf_8_2.c_str());
		}
		void ShadowRGBText(int x, int y, ImColor color, const char* str)
		{
			ImFont a;
			std::string utf_8_1 = std::string(str);
			std::string utf_8_2 = string_To_UTF8(utf_8_1);
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 240)), utf_8_2.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x + 1, y + 2), ImGui::ColorConvertFloat4ToU32(ImColor(0, 0, 0, 240)), utf_8_2.c_str());
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color)), utf_8_2.c_str());
		}
	};
}

MenuDefines::MenuDefines* UIDraw = new MenuDefines::MenuDefines();