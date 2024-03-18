#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include <TlHelp32.h>
#include "OptionDefine.h"

using namespace std;

struct WindowsFinderParams {
    DWORD pidOwner = NULL;
    std::wstring wndClassName = L"";
    std::wstring wndName = L"";
    RECT pos = { 0, 0, 0, 0 };
    POINT res = { 0, 0 };
    float percentAllScreens = 0.0f;
    float percentMainScreen = 0.0f;
    DWORD style = NULL;
    DWORD styleEx = NULL;
    bool satisfyAllCriteria = false;
    std::vector<HWND> hwnds;
};

namespace SetUp {

    HWND gWnd{}, DrawWnd{};

#define MAX_CLASSNAME 255
#define MAX_WNDNAME 255
#define TRANSPARENCY_COLOR RGBA(255,255,255,0)

    struct WindowsFinderParams {
        DWORD pidOwner = NULL;
        string wndClassName = "";
        string wndName = "";
        RECT pos = { 0, 0, 0, 0 };
        POINT res = { 0, 0 };
        float percentAllScreens = 0.0f;
        float percentMainScreen = 0.0f;
        DWORD style = NULL;
        DWORD styleEx = NULL;
        bool satisfyAllCriteria = false;
        vector<HWND> hwnds;
    };

    // Prototypes
    inline std::vector<HWND> WindowsFinder(WindowsFinderParams params);
    inline BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam);
    inline HWND HiJackNotepadWindow();
    inline std::vector<DWORD> GetPIDs(wstring targetProcessName);

    inline HWND HiJackNotepadWindow() {
        HWND hwnd = NULL;

        // Remove previous windows
        //vector<DWORD> existingNotepads = GetPIDs(L"Glass Browser.exe");
        //if (!existingNotepads.empty()) {
        //    for (int i(0); i < existingNotepads.size(); ++i) {
        //        // Terminating processes
        //        HANDLE hOldProcess = OpenProcess(PROCESS_TERMINATE, FALSE, existingNotepads[i]);
        //        TerminateProcess(hOldProcess, 0);
        //        CloseHandle(hOldProcess);
        //    }
        //}

        // Finding notepad's window (we could just use FindWindow but then it would be OS language dependent)
        vector<DWORD> notepads = GetPIDs(L"Glass Browser.exe");
        if (notepads.empty() || notepads.size() > 1) // Should check if more than one to be more strict
            return hwnd;
        WindowsFinderParams params;
        params.pidOwner = notepads[0];
        params.style = WS_VISIBLE;
        params.satisfyAllCriteria = true;
        vector<HWND> hwnds;
        int attempt = 0; // The process takes a bit of time to initialise and spawn the window, will try during 5 sec before time out
        while (hwnd == NULL || attempt > 50) {
            Sleep(100);
            hwnds = WindowsFinder(params);
            if (hwnds.size() > 1)
                return hwnd;
            hwnd = hwnds[0];
            ++attempt;
        }
        if (!hwnd)
            return hwnd;

        // Making the window usable for overlay puposes
        windef::WinWidth = GetSystemMetrics(SM_CXSCREEN);
        windef::WinHeight = GetSystemMetrics(SM_CYSCREEN);

        return hwnd;
    }

    inline std::vector<DWORD> GetPIDs(wstring targetProcessName) {
        vector<DWORD> pids;
        if (targetProcessName == L"")
            return pids;
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32W entry;
        entry.dwSize = sizeof entry;
        if (!Process32FirstW(snap, &entry)) {
            CloseHandle(snap);
            return pids;
        }
        do {
            if (wstring(entry.szExeFile) == targetProcessName) {
                pids.emplace_back(entry.th32ProcessID);
            }
        } while (Process32NextW(snap, &entry));
        CloseHandle(snap);
        return pids;
    }

    inline BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam) {
        WindowsFinderParams& params = *(WindowsFinderParams*)lParam;

        unsigned char satisfiedCriteria = 0, unSatisfiedCriteria = 0;

        // If looking for windows of a specific PDI
        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (params.pidOwner != NULL)
            if (params.pidOwner == pid)
                ++satisfiedCriteria; // Doesn't belong to the process targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows of a specific class
        char className[MAX_CLASSNAME] = "";
        GetClassNameA(hwnd, (LPSTR)className, MAX_CLASSNAME);
        string classNameWstr = className;
        if (params.wndClassName != "")
            if (params.wndClassName == classNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for windows with a specific name
        char windowName[MAX_WNDNAME] = "";
        GetWindowTextA(hwnd, (LPSTR)windowName, MAX_CLASSNAME);
        string windowNameWstr = windowName;
        if (params.wndName != "")
            if (params.wndName == windowNameWstr)
                ++satisfiedCriteria; // Not the class targeted
            else
                ++unSatisfiedCriteria;

        // If looking for window at a specific position
        RECT pos;
        GetWindowRect(hwnd, &pos);
        if (params.pos.left || params.pos.top || params.pos.right || params.pos.bottom)
            if (params.pos.left == pos.left && params.pos.top == pos.top && params.pos.right == pos.right && params.pos.bottom == pos.bottom)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for window of a specific size
        POINT res = { pos.right - pos.left, pos.bottom - pos.top };
        if (params.res.x || params.res.y)
            if (res.x == params.res.x && res.y == params.res.y)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage of all the screens
        float ratioAllScreensX = res.x / GetSystemMetrics(SM_CXSCREEN);
        float ratioAllScreensY = res.y / GetSystemMetrics(SM_CYSCREEN);
        float percentAllScreens = ratioAllScreensX * ratioAllScreensY * 100;
        if (params.percentAllScreens != 0.0f)
            if (percentAllScreens >= params.percentAllScreens)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // If looking for windows taking more than a specific percentage or the main screen
        RECT desktopRect;
        GetWindowRect(GetDesktopWindow(), &desktopRect);
        POINT desktopRes = { desktopRect.right - desktopRect.left, desktopRect.bottom - desktopRect.top };
        float ratioMainScreenX = res.x / desktopRes.x;
        float ratioMainScreenY = res.y / desktopRes.y;
        float percentMainScreen = ratioMainScreenX * ratioMainScreenY * 100;
        if (params.percentMainScreen != 0.0f)
            if (percentAllScreens >= params.percentMainScreen)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific styles
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        if (params.style)
            if (params.style & style)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        // Looking for windows with specific extended styles
        LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        if (params.styleEx)
            if (params.styleEx & styleEx)
                ++satisfiedCriteria;
            else
                ++unSatisfiedCriteria;

        if (!satisfiedCriteria)
            return TRUE;

        if (params.satisfyAllCriteria && unSatisfiedCriteria)
            return TRUE;

        // If looking for multiple windows
        params.hwnds.push_back(hwnd);
        return TRUE;
    }

    inline std::vector<HWND> WindowsFinder(WindowsFinderParams params) {
        EnumWindows(EnumWindowsCallback, (LPARAM)&params);
        return params.hwnds;
    }
}