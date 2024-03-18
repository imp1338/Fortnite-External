#pragma once

namespace MenuOptions {
	bool ShowWindow = true;
	bool StartWorld = false;
}
namespace FortOptions {

	// aimbot
	bool Aimbot = false;
	bool Humanize = false;
	
	// misc shit
	float AimFovValue = 70.0f;
	float Smoothing = 3.0f;
	bool AimFov = false;
	bool DrawCrosshair = false;


	//visuals
	bool DrawSnaplines = false;
	bool DrawPlayerCheck = false;
	bool DrawDistance = false;
	bool DisplayPlatform = false;
	bool DrawUsername = false;
	bool BoxEsp = false;
	bool CornerBoxEsp = false;
	bool Skeleton = false;
	bool DrawWeapon = false;
	bool HeadBox = false;
	bool HeadCirlce = false;
}

namespace windef {
	static HWND Window = NULL;
	IDirect3D9Ex* DirectObject = NULL;
	static LPDIRECT3DDEVICE9 D3DDevice = NULL;
	static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;
	RECT GameRect = { NULL };
	D3DPRESENT_PARAMETERS PresentParams;
	DWORD ScreenCenterX;
	DWORD ScreenCenterY;
	ImFont* m_pFont;
	DWORD WinWidth;
	DWORD WinHeight;
	MSG Message = { NULL };
}

namespace fort_define {
	HWND FortHwnd = NULL;
}
