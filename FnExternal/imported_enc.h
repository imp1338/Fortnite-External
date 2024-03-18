#include <minwindef.h>
#include <libloaderapi.h>
#include <memoryapi.h>
#include "string_enc.hpp"
#include "func_enc.h"
#include <WinUser.h>

namespace Imported { static BYTE NtUserSendInput_Bytes[30]; static BYTE NtUserGetAsyncKeyState_Bytes[30]; static BOOLEAN WINAPI Init() { LPVOID NtUserSendInput_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("user32").decrypt()), E("NtUserSendInput").decrypt()); if (!NtUserSendInput_Addr) { NtUserSendInput_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("user32").decrypt()), E("NtUserSendInput").decrypt()); if (!NtUserSendInput_Addr) { NtUserSendInput_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("user32").decrypt()), E("SendInput").decrypt()); if (!NtUserSendInput_Addr) return FALSE; } } LPVOID NtUserGetAsyncKeyState_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("win32u").decrypt()), E("NtUserGetAsyncKeyState").decrypt()); if (!NtUserGetAsyncKeyState_Addr) { NtUserGetAsyncKeyState_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("user32").decrypt()), E("NtUserGetAsyncKeyState").decrypt()); if (!NtUserGetAsyncKeyState_Addr) { NtUserGetAsyncKeyState_Addr = LI_FN(GetProcAddress).get()(GetModuleHandle(E("user32").decrypt()), E("GetAsyncKeyState").decrypt()); if (!NtUserGetAsyncKeyState_Addr) return FALSE; } } LI_FN(memcpy).get()(NtUserSendInput_Bytes, NtUserSendInput_Addr, 30); LI_FN(memcpy).get()(NtUserGetAsyncKeyState_Bytes, NtUserGetAsyncKeyState_Addr, 30); return TRUE; } static BOOLEAN WINAPI NtUserSendInput(UINT cInputs, LPINPUT pInputs, int cbSize) { LPVOID NtUserSendInput_Spoof = LI_FN(VirtualAlloc).get()(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE); if (!NtUserSendInput_Spoof) return FALSE; LI_FN(memcpy).get()(NtUserSendInput_Spoof, NtUserSendInput_Bytes, 30); NTSTATUS Result = reinterpret_cast<NTSTATUS(NTAPI*)(UINT, LPINPUT, int)>(NtUserSendInput_Spoof)(cInputs, pInputs, cbSize);  ZeroMemory(NtUserSendInput_Spoof, 0x1000); LI_FN(VirtualFree).get()(NtUserSendInput_Spoof, 0, MEM_RELEASE);  return (Result > 0);  } static UINT WINAPI NtUserGetAsyncKeyState(UINT Key) { LPVOID NtUserGetAsyncKeyState_Spoof = LI_FN(VirtualAlloc).get()(0, 0x1000, MEM_COMMIT, PAGE_EXECUTE_READWRITE); if (!NtUserGetAsyncKeyState_Spoof) return FALSE; LI_FN(memcpy).get()(NtUserGetAsyncKeyState_Spoof, NtUserGetAsyncKeyState_Bytes, 30); NTSTATUS Result = reinterpret_cast<NTSTATUS(NTAPI*)(UINT)>(NtUserGetAsyncKeyState_Spoof)(Key);  ZeroMemory(NtUserGetAsyncKeyState_Spoof, 0x1000); LI_FN(VirtualFree).get()(NtUserGetAsyncKeyState_Spoof, 0, MEM_RELEASE);  return Result; } static BOOLEAN WINAPI Move_Mouse(int X, int Y) { INPUT input; input.type = INPUT_MOUSE; input.mi.mouseData = 0;input.mi.time = 0;input.mi.dx = X;input.mi.dy = Y;input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK;return NtUserSendInput(1, &input, sizeof(input));} static UINT WINAPI GetAsyncKeyState(UINT Key){return NtUserGetAsyncKeyState(Key);} }

static DWORD Get_VK_Insert_Fraps() {

    HKEY                 hKey{};
    DWORD                dwValue{};

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Fraps3", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dwSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, "Overlay Key", nullptr, nullptr, reinterpret_cast<LPBYTE>(&dwValue), &dwSize) != ERROR_SUCCESS) {
            dwValue = 0;
        }
        RegCloseKey(hKey);
    }
    return dwValue;
}

static DWORD Get_VK_RBUTTON_Fraps() {

    HKEY                 hKey{};
    DWORD                dwValue{};

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Fraps3", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD dwSize = sizeof(DWORD);
        if (RegQueryValueEx(hKey, "Movie Key", nullptr, nullptr, reinterpret_cast<LPBYTE>(&dwValue), &dwSize) != ERROR_SUCCESS) {
            dwValue = 0;
        }
        RegCloseKey(hKey);
    }
    return dwValue;
}