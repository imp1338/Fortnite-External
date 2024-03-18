#pragma once
#include "includes.h"

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
const MARGINS Margin = { -1 };

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace D3D {

	void create_d3d()
	{
		if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &windef::DirectObject)))
			exit(3);

		ZeroMemory(&windef::PresentParams, sizeof(windef::PresentParams));
		windef::PresentParams.BackBufferWidth = windef::WinWidth;
		windef::PresentParams.BackBufferHeight = windef::WinWidth;
		windef::PresentParams.BackBufferFormat = D3DFMT_A8R8G8B8;
		windef::PresentParams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
		windef::PresentParams.AutoDepthStencilFormat = D3DFMT_D16;
		windef::PresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		windef::PresentParams.EnableAutoDepthStencil = TRUE;
		windef::PresentParams.hDeviceWindow = windef::Window;
		windef::PresentParams.Windowed = TRUE;
		windef::PresentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

		windef::DirectObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windef::Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &windef::PresentParams, &windef::D3DDevice);

		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		(void)io;

		ImGui_ImplWin32_Init(windef::Window);
		ImGui_ImplDX9_Init(windef::D3DDevice);

		ImGui::StyleColorsClassic();
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowBorderSize = 0;


		style->WindowTitleAlign.x = 0.50f;

		XorS(font, "C:\\Windows\\Font\\segoui.ttf");
		windef::m_pFont = io.Fonts->AddFontFromFileTTF(font.decrypt(), 12.0f, nullptr, io.Fonts->GetGlyphRangesDefault());

		windef::DirectObject->Release();
	}
}

namespace Renderer {

	void draw_loop() {

		Cache::World = read<uintptr_t>(Cache::BaseAddress + Addresses::GWorld);
		Cache::GameInstance = read<uintptr_t>(Cache::World + Offsets::GameInstance);
		Cache::LocalPlayers = read<uintptr_t>(read<uintptr_t>(Cache::GameInstance + Offsets::LocalPlayers));
		Cache::LocalPlayer = read<uintptr_t>(Cache::LocalPlayers);
		Cache::PlayerController = read<uintptr_t>(Cache::LocalPlayer + Offsets::PlayerController);
		Cache::LocalPawn = read<uintptr_t>(Cache::PlayerController + Offsets::LocalPawn);
		if (Cache::LocalPawn)
		{
			Cache::PState = read<uintptr_t>(Cache::LocalPawn + Offsets::PlayerState);
			Cache::RootComponent = read<uintptr_t>(Cache::LocalPawn + Offsets::RootC);
			Cache::PersistentState = read<uintptr_t>(Cache::World + Offsets::PersistentLevel);
			Cache::LocalActorPosition = read<Vector3>(Cache::RootComponent + 0x120);
		}
		Cache::GameState = read<DWORD_PTR>(Cache::World + Offsets::GameState);
		Cache::PlayerArray = read<DWORD_PTR>(Cache::GameState + Offsets::PlayerArray);

		float closestDistance = FLT_MAX;
		uintptr_t closestPawn = NULL;
		static const auto size = ImGui::GetIO().DisplaySize;
		static const auto center = ImVec2(size.x / 2, size.y / 2);


		// read array
		int ActorArray = read<int>(Cache::GameState + (Offsets::PlayerArray + sizeof(uintptr_t)));


		//std::cout << ActorArray << std::endl;
		//// loop array
		for (uint32_t i = 0; i < ActorArray; i++)
		{


			auto ArrayPlayer = read<uintptr_t>(Cache::PlayerArray + i * 0x8);
			auto LocalActor = read<uintptr_t>(ArrayPlayer + 0x308);
		
			if (!LocalActor) {
				continue;
			}

			int NewPlayerLocationX;
			int NewPlayerLocationY;

			uint64_t CurrentActorMesh = read<uint64_t>(LocalActor + Offsets::ActorMesh);
			int MyTeamId = read<int>(Cache::PState + Offsets::TIndex);
			DWORD64 otherPlayerState = read<uint64_t>(LocalActor + Offsets::PlayerState);
			int OtherTeamId = read<int>(otherPlayerState + Offsets::TIndex);
			auto isDBNO = (read<char>(LocalActor + 0x93a) >> 4) & 1;

			if (LocalActor == Cache::LocalPawn)
				continue;


			ImColor VisibleBox = ImColor(0, 255, 0);
			ImColor InvisibleBox = ImColor(255, 0, 0);

			ImColor VisibleSkeleton = ImColor(255, 0, 0);
			ImColor InvisibleSkeleton = ImColor(255, 255, 0);

			Vector3 RotatedCenterHead = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, 66);
			Vector3 HEAD = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, FortBoneArray_Defines::head);
			Vector3 ROOT = FortBoneArray_Functions::GetBoneWithRotation(CurrentActorMesh, FortBoneArray_Defines::Root);
			Vector3 ProjectedActorRoot = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(ROOT.x,ROOT.y,ROOT.z - 7));
			Vector3 ProjectedActorHead = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(HEAD.x, HEAD.y, HEAD.z + 22));
			Vector3 ProjectedActorAim = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(HEAD.x, HEAD.y, HEAD.z));
			Vector3 ProjectedActorCenter = FortViewPoint_Functions::ProjectWorldToScreen(Vector3(RotatedCenterHead.x - .25, RotatedCenterHead.y, RotatedCenterHead.z + 15));
			bool bIsABot = read<bool>(ArrayPlayer + 0x29a);


			auto PlayerRelativeLocation = read<Vector3>(LocalActor + 0x128);

			float PlayerDistance = Cache::LocalActorPosition.Distance(PlayerRelativeLocation) / 80;

			int center_x = GetSystemMetrics(0) / 2 - 3;
			int center_y = GetSystemMetrics(1) / 2 - 3;

			float CornerHeight = abs(ProjectedActorHead.y - ProjectedActorRoot.y);
			float CornerWidth = CornerHeight * .530;

			auto dx = ProjectedActorAim.x - (size.x / 2);
			auto dy = ProjectedActorAim.y - (size.y / 2);
			auto dist = sqrtf(dx * dx + dy * dy);

			if (dist < 120 && dist < closestDistance) {
				closestDistance = dist;
				closestPawn = LocalActor;
			}

			float YOffset = 0;

			if (FortOptions::BoxEsp)
			{
				DrawBox(ProjectedActorHead.x - (CornerWidth / 2), ProjectedActorHead.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), 4, 2);
				DrawBox(ProjectedActorHead.x - (CornerWidth / 2), ProjectedActorHead.y, CornerWidth, CornerHeight, ImColor(0, 255, 0), 1, 2);
			}
			if (FortOptions::CornerBoxEsp)
			{
				DrawCorneredBox(ProjectedActorHead.x - (CornerWidth / 2), ProjectedActorHead.y, CornerWidth, CornerHeight, ImColor(0, 0, 0), 4);
				DrawCorneredBox(ProjectedActorHead.x - (CornerWidth / 2), ProjectedActorHead.y, CornerWidth, CornerHeight, ImColor(0, 255, 0), 1);
			}
			if (FortOptions::Skeleton)
			{
				DrawSkeleton(CurrentActorMesh, ImColor(0,0,0),4 );
				DrawSkeleton(CurrentActorMesh, ImColor(255, 0, 0), 1);
			}
			if (FortOptions::HeadBox)
			{
				EDrawings->DrawThreeDimensionalBox(CurrentActorMesh, 67, 109, 23, NULL);
			}
			if (FortOptions::HeadCirlce)
			{
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ProjectedActorCenter.x, ProjectedActorCenter.y), 45, ImColor(0, 0, 0), 30,4);
				ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ProjectedActorCenter.x, ProjectedActorCenter.y), 45, ImColor(255, 0, 0), 30);
			}
			if (FortOptions::DrawSnaplines)
				ImGui::GetBackgroundDrawList()->AddLine(ImVec2(size.x / 2, 0), ImVec2(ProjectedActorHead.x, ProjectedActorHead.y), ImColor(255, 255, 255));
			if (FortOptions::DrawDistance)
			{
				char distance[64];
				sprintf_s(distance, E("Distance -> %2.f"), (PlayerDistance));
				YOffset = YOffset + 13;
				DrawString(13, ProjectedActorHead.x - 13, ProjectedActorHead.y - YOffset, ImColor(255,255,255), true, true, distance);
			}
			if (FortOptions::DisplayPlatform)
			{
				auto PlayerPlatform = APlayerState->ReturnPlatform(ArrayPlayer);
				char platform[64];
				sprintf_s(platform, E("Platform -> %s"), PlayerPlatform);
				YOffset = YOffset + 13;
				DrawString(13, ProjectedActorHead.x - 13, ProjectedActorHead.y - YOffset, ImColor(255, 255, 255), true, true, platform);
			}
			if (FortOptions::DrawUsername)
			{
				auto PlayerUsername = APlayerState->username(ArrayPlayer);
				char username[64];
				sprintf_s(username, E("Username -> %s"), PlayerUsername);
				YOffset = YOffset + 13;
				DrawString(13, ProjectedActorHead.x - 13, ProjectedActorHead.y - YOffset, ImColor(255, 255, 255), true, true, username);
			}
			if (FortOptions::DrawPlayerCheck)
			{
				if (bIsABot)
				{
					char BotCheck[64];
					sprintf_s(BotCheck, E("BOT / NPC"));
					YOffset = YOffset + 13;
					DrawString(13, ProjectedActorHead.x - 9, ProjectedActorHead.y - YOffset, ImColor(255, 0, 0), true, true, BotCheck);
				}
			}	

			if (FortOptions::Aimbot && closestPawn && Imported::GetAsyncKeyState(Get_VK_RBUTTON_Fraps()) > 0)
			{
				uintptr_t ClosestActorMesh = read<uintptr_t>(closestPawn + Offsets::ActorMesh);
				if (!ClosestActorMesh)
					return;

				Vector3 ClosestActorHead = FortBoneArray_Functions::GetBoneWithRotation(ClosestActorMesh, FortBoneArray_Defines::head);
				if (ClosestActorHead.x < 1)
					return;

				Vector3 ClosestActorHeadProjected = FortViewPoint_Functions::ProjectWorldToScreen(ClosestActorHead);
				if (ClosestActorHeadProjected.x < 1)
					return;

				if (ClosestActorHeadProjected.x != 0 || ClosestActorHeadProjected.y != 0)
					MOUClass->SmoothMouseMovement(ClosestActorHeadProjected.x, ClosestActorHeadProjected.y);
			}
		}
	}

	int Selection = 1;

	void FilterSelectedItemSelection(int item)
	{
		switch (item)
		{
		case 1:FortOptions::Aimbot = !FortOptions::Aimbot;break;
		case 2:FortOptions::Humanize = !FortOptions::Humanize;break;
		case 3:FortOptions::AimFov = !FortOptions::AimFov; break;
		case 4:FortOptions::DrawCrosshair = !FortOptions::DrawCrosshair;break;
		case 7: FortOptions::BoxEsp = !FortOptions::BoxEsp; break;
		case 8: FortOptions::CornerBoxEsp = !FortOptions::CornerBoxEsp; break;
		case 9: FortOptions::Skeleton = !FortOptions::Skeleton; break;
		case 10: FortOptions::DrawDistance = !FortOptions::DrawDistance; break;
		case 11: FortOptions::DrawPlayerCheck = !FortOptions::DrawPlayerCheck; break;
		case 12: FortOptions::DisplayPlatform = !FortOptions::DisplayPlatform; break;
		case 13: FortOptions::DrawSnaplines = !FortOptions::DrawSnaplines; break;
		case 14: FortOptions::HeadBox = !FortOptions::HeadBox; break;
		case 15: FortOptions::HeadCirlce = !FortOptions::HeadCirlce; break;
		case 16: FortOptions::DrawUsername = !FortOptions::DrawUsername; break;
		case 17: FortOptions::DrawWeapon = !FortOptions::DrawWeapon; break;
	    }
	}

	void menu_loop()
	{

		ImGuiStyle* style = &ImGui::GetStyle();
		style->Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);                      // Text color (white)
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);              // Disabled text color (white)
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.0f);                // Window background color (dark grey)
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);                 // Title bar background color (light purple)
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);           // Active title bar background color (light purple)
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);        // Collapsed title bar background color (light purple)
		style->Colors[ImGuiCol_Button] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);                   // Button color (dark grey)
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);               // Button hover color (darker grey)
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.2f, 0.2f, 0.6f);                // Button active color (darker grey)
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);                    // Frame background color (semi-transparent dark grey)
		style->Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);              // Frame background hover color (darker grey)
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);               // Active title bar background color (darker grey)

		int drX = windef::WinWidth / 2;
		int drY = windef::WinHeight / 2;

		if (FortOptions::DrawCrosshair)
		{
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY), ImVec2(drX, drY - 15), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY - 15), ImVec2(drX + 15, drY - 15), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY), ImVec2(drX + 15, drY), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX + 15, drY), ImVec2(drX + 15, drY + 15), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY), ImVec2(drX, drY + 15), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY + 15), ImVec2(drX - 15, drY + 15), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX, drY), ImVec2(drX - 15, drY), ImColor(255, 0, 0));
			ImGui::GetBackgroundDrawList()->AddLine(ImVec2(drX - 15, drY), ImVec2(drX - 15, drY - 15), ImColor(255, 0, 0));
		}

		if (FortOptions::AimFov)
		{
			ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(drX, drY),FortOptions::AimFovValue, ImColor(0,0,0,100), 30.0f);
		}

		char storage_pointer[256];
		sprintf_s(storage_pointer, E("feds.lol/soarcheats <-> Fortnite <-> Frame Rate: %.1f").decrypt(), ImGui::GetIO().Framerate);
		ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), ImColor(225, 255, 255), storage_pointer);

		if (Imported::GetAsyncKeyState(VK_INSERT) & 1)
		{
			MenuOptions::ShowWindow = !MenuOptions::ShowWindow;
		}
		if (MenuOptions::ShowWindow)
		{
			if (GetAsyncKeyState(VK_DOWN) & 1)
				Selection = Selection + 1;

			if (GetAsyncKeyState(VK_UP) & 1)
				Selection = Selection - 1;

			if (GetAsyncKeyState(VK_RETURN) & 1)
				FilterSelectedItemSelection(Selection);



			// aim fov left right value adjuster
			if (GetAsyncKeyState(VK_LEFT) && Selection == 5)
				FortOptions::AimFovValue = FortOptions::AimFovValue - 0.25;

			if (GetAsyncKeyState(VK_RIGHT) && Selection == 5)
				FortOptions::AimFovValue = FortOptions::AimFovValue + 0.25;


			// smoothing left right value adjuster
			if (GetAsyncKeyState(VK_LEFT) && Selection == 6)
				FortOptions::Smoothing = FortOptions::Smoothing - 0.01;

			if (GetAsyncKeyState(VK_RIGHT) && Selection == 6)
				FortOptions::Smoothing = FortOptions::Smoothing + 0.01;

			if (Selection < 1)
				Selection = 16;
			if (Selection > 16)
				Selection = 1;


			windef::ScreenCenterX = 10;


			if (FortOptions::Smoothing < 1)
			{
				FortOptions::Smoothing = 1;
			}
			if (FortOptions::AimFovValue < 1)
			{
				FortOptions::AimFovValue = 1;
			}

			if (FortOptions::Smoothing > 20)
			{
				FortOptions::Smoothing = 20;
			}
			if (FortOptions::AimFovValue > 600)
			{
				FortOptions::AimFovValue = 600;
			}


			// selection defines

			const char* Key_1 = E("");
			const char* Key_2 = E("");
			const char* Key_3 = E("");
			const char* Key_4 = E("");
			const char* Key_5 = E("");
			const char* Key_6 = E("");
			const char* Key_7 = E("");
			const char* Key_8 = E("");
			const char* Key_9 = E("");
			const char* Key_10 = E("");
			const char* Key_11 = E("");
			const char* Key_12 = E("");
			const char* Key_13 = E("");
			const char* Key_14 = E("");
			const char* Key_15 = E("");
			const char* Key_16 = E("");
			const char* Key_17 = E("");
			// selection initilizatation

			switch (Selection)
			{
			case 1: Key_1 = E(" >"); break;
			case 2: Key_2 = E(" >"); break;
			case 3: Key_3 = E(" >"); break;
			case 4: Key_4 = E(" >"); break;
			case 5: Key_5 = E(" >"); break;
			case 6: Key_6 = E(" >"); break;
			case 7: Key_7 = E(" >"); break;
			case 8: Key_8 = E(" >"); break;
			case 9: Key_9 = E(" >"); break;
			case 10: Key_10 = E(" >"); break;
			case 11: Key_11 = E(" >"); break;
			case 12: Key_12 = E(" >"); break;
			case 13: Key_13 = E(" >"); break;
			case 14: Key_14 = E(" >"); break;
			case 15: Key_15 = E(" >"); break;
			case 16: Key_16 = E(" >"); break;
			case 17: Key_17 = E(" >"); break;
			}



			// render menu main

			SimpleText(windef::ScreenCenterX, 40, ImColor(255, 255, 255), "PornHook.cc V1.2");
			SimpleText(windef::ScreenCenterX, 60, ImColor(255, 255, 255), "");
			SimpleText(windef::ScreenCenterX, 80, ImColor(255, 255, 255), "Aimbot Options:");
			SimpleBox(windef::ScreenCenterX, 100, ImColor(255, 255, 255, 255), "Mouse Aim", Key_1, FortOptions::Aimbot);
			SimpleBox(windef::ScreenCenterX, 120, ImColor(255, 255, 255, 255), "Huamnize Aim", Key_2, FortOptions::Humanize);
			SimpleBox(windef::ScreenCenterX, 140, ImColor(255, 255, 255, 255), "Field of View Circle", Key_3, FortOptions::AimFov);
			SimpleBox(windef::ScreenCenterX, 160, ImColor(255, 255, 255, 255), "Crosshair", Key_4, FortOptions::DrawCrosshair);
			SimpleBoxValue(windef::ScreenCenterX, 180, ImColor(255, 255, 255, 255), "Aim Field of View", Key_5, FortOptions::AimFovValue);
			SimpleBoxValue(windef::ScreenCenterX, 200, ImColor(255, 255, 255, 255), "Smoothing Value", Key_6, FortOptions::Smoothing);
			SimpleText(windef::ScreenCenterX, 220, ImColor(255, 255, 255), "");
			SimpleText(windef::ScreenCenterX, 240, ImColor(255, 255, 255), "Visual Options:");
			SimpleBox(windef::ScreenCenterX, 260, ImColor(255, 255, 255, 255), "Box", Key_7, FortOptions::BoxEsp);
			SimpleBox(windef::ScreenCenterX, 280, ImColor(255, 255, 255, 255), "Corner Box", Key_8, FortOptions::CornerBoxEsp);
			SimpleBox(windef::ScreenCenterX, 300, ImColor(255, 255, 255, 255), "Skeletel Meshes", Key_9, FortOptions::Skeleton);
			SimpleBox(windef::ScreenCenterX, 320, ImColor(255, 255, 255, 255), "Distances (Meters)", Key_10, FortOptions::DrawDistance);
			SimpleBox(windef::ScreenCenterX, 340, ImColor(255, 255, 255, 255), "AI / BOT Identifier", Key_11, FortOptions::DrawPlayerCheck);
			SimpleBox(windef::ScreenCenterX, 360, ImColor(255, 255, 255, 255), "Platform", Key_12, FortOptions::DisplayPlatform);
			SimpleBox(windef::ScreenCenterX, 380, ImColor(255, 255, 255, 255), "Snaplines", Key_13, FortOptions::DrawSnaplines);

			SimpleBox(windef::ScreenCenterX, 400, ImColor(255, 255, 255, 255), "Head Box", Key_14, FortOptions::HeadBox);		
			SimpleBox(windef::ScreenCenterX, 420, ImColor(255, 255, 255, 255), "Head Circle", Key_15, FortOptions::HeadCirlce);


			SimpleBox(windef::ScreenCenterX, 440, ImColor(255, 255, 255, 255), "Username", Key_16, FortOptions::DrawUsername);
			SimpleBox(windef::ScreenCenterX, 460, ImColor(255, 255, 255, 255), "Weapon", Key_17, FortOptions::DrawWeapon);


			/*ImGui::SetNextWindowSize(ImVec2(450, 500));
			ImGui::Begin("Fortnite");
			ImGui::Spacing();


			ImGui::Text("Aim Options");
			ImGui::Separator();
			ImGui::Checkbox("Mouse Aim", &FortOptions::Aimbot);
			ImGui::Checkbox("Humanize Aim", &FortOptions::Humanize);
			ImGui::SliderFloat("Aim FOV", &FortOptions::AimFovValue,10.0f,750.0f);
			ImGui::SliderFloat("Aim Smoothing", &FortOptions::Smoothing, 1.0f,20.0f);
			ImGui::Spacing();


			ImGui::Text("Visuals");
			ImGui::Separator();
			ImGui::Checkbox("Draw Box", &FortOptions::BoxEsp);
			ImGui::Checkbox("Draw Cornered Box", &FortOptions::CornerBoxEsp);
			ImGui::Checkbox("Draw Skeleton", &FortOptions::Skeleton);
			ImGui::Checkbox("Draw Snaplines", &FortOptions::DrawSnaplines);
			ImGui::Checkbox("Draw Distance", &FortOptions::DrawDistance);
			ImGui::Checkbox("Draw Player Check", &FortOptions::DrawPlayerCheck);
			ImGui::Checkbox("Draw Platform", &FortOptions::DisplayPlatform);
			ImGui::Checkbox("Draw Username", &FortOptions::DrawUsername);
			ImGui::Spacing();


			ImGui::Text("Misc");
			ImGui::Separator();
			ImGui::Checkbox("Draw FOV Circle", &FortOptions::AimFov);
			ImGui::Checkbox("Draw Crosshair", &FortOptions::DrawCrosshair);
			ImGui::End();*/
		}
	}

	void render()
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		draw_loop();
		menu_loop();

		ImGui::EndFrame();
		windef::D3DDevice->SetRenderState(D3DRS_ZENABLE, false);
		windef::D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		windef::D3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		windef::D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		if (windef::D3DDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			windef::D3DDevice->EndScene();
		}
		HRESULT result = windef::D3DDevice->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && windef::D3DDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			windef::D3DDevice->Reset(&windef::PresentParams);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}
}

namespace WindowStartup {

	static LRESULT CALLBACK window_handler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

	void render_loop()
	{
		static RECT old_rc;
		RtlSecureZeroMemory(&windef::Message, sizeof(MSG));

		while (windef::Message.message != WM_QUIT)
		{
			if (PeekMessage(&windef::Message, windef::Window, 0, 0, PM_REMOVE))
			{
				LI_FN(TranslateMessage).get()(&windef::Message);
				DispatchMessage(&windef::Message);
			}

			HWND hwnd_active = LI_FN(GetForegroundWindow).get()();

			if (hwnd_active == fort_define::FortHwnd) {
				HWND hwndtest = LI_FN(GetWindow).get()(hwnd_active, GW_HWNDPREV);
				LI_FN(SetWindowPos).get()(windef::Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			if (Imported::GetAsyncKeyState(0x23) & 1)
				exit(8);

			RECT rc;
			POINT xy;
			RtlSecureZeroMemory(&rc, sizeof(RECT));
			RtlSecureZeroMemory(&xy, sizeof(POINT));
			LI_FN(GetClientRect).get()(fort_define::FortHwnd, &rc);
			LI_FN(ClientToScreen).get()(fort_define::FortHwnd, &xy);
			rc.left = xy.x;
			rc.top = xy.y;

			ImGuiIO& io = ImGui::GetIO();
			io.IniFilename = NULL;
			io.ImeWindowHandle = fort_define::FortHwnd;
			io.DeltaTime = 1.0f / 60.0f;

			POINT p;
			GetCursorPos(&p);
			io.MousePos.x = p.x - xy.x;
			io.MousePos.y = p.y - xy.y;

			if (Imported::GetAsyncKeyState(0x1))
			{
				io.MouseDown[0] = true;
				io.MouseClicked[0] = true;
				io.MouseClickedPos[0].x = io.MousePos.x;
				io.MouseClickedPos[0].x = io.MousePos.y;
			}
			else
			{
				io.MouseDown[0] = false;
			}

			if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
			{
				old_rc = rc;

				windef::WinWidth = rc.right;
				windef::WinHeight = rc.bottom;

				windef::PresentParams.BackBufferWidth = windef::WinWidth;
				windef::PresentParams.BackBufferHeight = windef::WinHeight;
				LI_FN(SetWindowPos).get()(windef::Window, (HWND)0, xy.x, xy.y, windef::WinWidth, windef::WinHeight, SWP_NOREDRAW);
				windef::D3DDevice->Reset(&windef::PresentParams);
			}

			if (fort_define::FortHwnd)
			{
				ZeroMemory(&windef::GameRect, sizeof(windef::GameRect));
				LI_FN(GetWindowRect).get()(fort_define::FortHwnd, &windef::GameRect);
				windef::WinWidth = windef::GameRect.right - windef::GameRect.left;
				windef::WinHeight = windef::GameRect.bottom - windef::GameRect.top;
				DWORD dwStyle = GetWindowLong(fort_define::FortHwnd, GWL_STYLE);

				if (dwStyle & WS_BORDER)
				{
					windef::GameRect.top += 32;
					windef::WinHeight -= 39;
				}
				windef::ScreenCenterX = windef::WinWidth / 2;
				windef::ScreenCenterY = windef::WinHeight / 2;
				LI_FN(MoveWindow).safe()(windef::Window, windef::GameRect.left, windef::GameRect.top, windef::WinWidth, windef::WinHeight, true);
			}
			else
			{
				exit(0);
			}

			Renderer::render();
		}
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		LI_FN(DestroyWindow).get()(windef::Window);
	}

	void shutdownWin()
	{
		windef::TriBuf->Release();
		windef::D3DDevice->Release();
		windef::DirectObject->Release();

		LI_FN(DestroyWindow).get()(windef::Window);
		UnregisterClass("Fortnite  ", NULL);
	}


	LRESULT CALLBACK window_handler(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
			return true;

		switch (Message)
		{
		case WM_DESTROY:
			shutdownWin();
			LI_FN(PostQuitMessage).get()(0);
			LI_FN(exit).get()(4);
			break;
		case WM_SIZE:
			if (windef::D3DDevice != NULL && wParam != SIZE_MINIMIZED)
			{
				ImGui_ImplDX9_InvalidateDeviceObjects();
				windef::PresentParams.BackBufferWidth = LOWORD(lParam);
				windef::PresentParams.BackBufferHeight = HIWORD(lParam);
				HRESULT hr = windef::D3DDevice->Reset(&windef::PresentParams);
				if (hr == D3DERR_INVALIDCALL)
					IM_ASSERT(0);
				ImGui_ImplDX9_CreateDeviceObjects();
			}
			break;
		default:
			return DefWindowProc(hWnd, Message, wParam, lParam);
			break;
		}
		return 0;
	}

	void create_overlay()
	{

		WNDCLASS windowClass = { 0 };
		windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hInstance = NULL;
		windowClass.lpfnWndProc = window_handler;
		windowClass.lpszClassName = "Fortnite";
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		RegisterClass(&windowClass);
		windef::Window = CreateWindow("Fortnite", "Fortnite", WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, NULL, NULL);
		LI_FN(ShowWindow).get()(windef::Window, SW_SHOW);
		LI_FN(DwmExtendFrameIntoClientArea).get()(windef::Window, &Margin);
		LI_FN(SetWindowLongA).get()(windef::Window, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
		LI_FN(UpdateWindow).get()(windef::Window);
	}

	namespace DirectX9Interface
	{
		IDirect3D9Ex* p_object = NULL;
		IDirect3DDevice9Ex* p_device = NULL;
		D3DPRESENT_PARAMETERS p_params = { NULL };
		RECT game_rect = { NULL };
		MSG message = { NULL };
		HWND hijacked_hwnd = NULL;
		HWND game_wnd = NULL;
	}

	namespace hijack
	{
		bool amd = true;
		bool nvidia = false;

		__forceinline void hijack_init()
		{
			if (LI_FN(FindWindowA)(E("GDI+ Hook Window Class"), E("GDI+ Window (obs64.exe)")))
			{
					amd = true;
				DirectX9Interface::hijacked_hwnd = LI_FN(FindWindowA)(E("GDI+ Hook Window Class"), E("GDI+ Window (obs64.exe)"));
				NOTIFYICONDATA nid = {};
				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = NULL;
				nid.uID = 1;
				nid.uFlags = NIF_INFO;
				nid.dwInfoFlags = NIIF_INFO;

				Shell_NotifyIcon(NIM_ADD, &nid);
				Shell_NotifyIcon(NIM_DELETE, &nid);
			}
			else
			{
					NOTIFYICONDATA nid = {};
				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = NULL;
				nid.uID = 1;
				nid.uFlags = NIF_INFO;
				nid.dwInfoFlags = NIIF_INFO;


				MessageBox(NULL, "Lightshot Could not be Found Running.\nError : 0x00000b2a", NULL, MB_OK);
				Shell_NotifyIcon(NIM_ADD, &nid);
				Shell_NotifyIcon(NIM_DELETE, &nid);
				exit(1);
			}

			LI_FN(ShowWindow)(DirectX9Interface::hijacked_hwnd, SW_SHOW);
			SetWindowLongPtrA(windef::Window, GWLP_WNDPROC, (LONG_PTR)window_handler);
			LI_FN(SetWindowLongA)(DirectX9Interface::hijacked_hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
			LI_FN(SetWindowLongA)(
				DirectX9Interface::hijacked_hwnd,
				-20,
				static_cast<LONG_PTR>(
					static_cast<int>(LI_FN(GetWindowLongA)(DirectX9Interface::hijacked_hwnd, -20)) | 0x20
					)
				);

			windef::WinWidth = GetSystemMetrics(SM_CXSCREEN);
			windef::WinHeight = GetSystemMetrics(SM_CYSCREEN);


			if (amd) LI_FN(MoveWindow)(DirectX9Interface::hijacked_hwnd, 0, 0, windef::WinWidth, windef::WinHeight, false); // only when amd

			//transparency
			MARGINS margin = { -1, -1, -1, -1 };
			LI_FN(DwmExtendFrameIntoClientArea)(
				DirectX9Interface::hijacked_hwnd,
				&margin
				);

			LI_FN(SetLayeredWindowAttributes)(
				DirectX9Interface::hijacked_hwnd,
				NULL,
				0xFF,
				0x02
				);

			// top most
			LI_FN(SetWindowPos)(
				DirectX9Interface::hijacked_hwnd,
				HWND_TOPMOST,
				0, 0, 0, 0,
				0x0002 | 0x0001
				);


			LI_FN(UpdateWindow)(DirectX9Interface::hijacked_hwnd);
			windef::Window = DirectX9Interface::hijacked_hwnd;

			//LI_FN(CreateThread).get()(0, 0, (LPTHREAD_START_ROUTINE)loop_window, 0, 0, 0);
		}

	}
}

namespace Overlay {
	void StartOverlay()
	{
		// create overlay
       // WindowStartup::hijack::hijack_init();
		WindowStartup::create_overlay();

		// create directx drawing tablet
		D3D::create_d3d();
		// render main loop
		WindowStartup::render_loop();

		// if render is returned, unregister class and win
		WindowStartup::shutdownWin();
	}
}