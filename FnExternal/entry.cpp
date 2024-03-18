#include <iostream>
#include "OverlayDefines.h"
#include "driver.hh"
#include "console_drawing.h"

int main()
{
    HWND consoleWindow = LI_FN(GetConsoleWindow).get()();
    LONG_PTR windowStyle = GetWindowLongPtr(consoleWindow, GWL_EXSTYLE);
    SetWindowLongPtr(consoleWindow, GWL_EXSTYLE, windowStyle | WS_EX_LAYERED);
    LI_FN(SetLayeredWindowAttributes).get()(consoleWindow, 0, 240, LWA_ALPHA);

    LI_FN(SetConsoleTitleA).get()(E("feds.lol/soarcheats | discord.gg/synapsefn").decrypt());

    std::cout << std::endl;
    ConsoleUtils::WriteLine(E("soarcheats waz here :)").decrypt());
    ConsoleUtils::WriteLine(E("start fortnut...").decrypt());

    while (fort_define::FortHwnd == NULL)
        fort_define::FortHwnd = LI_FN(FindWindowA).get()(0, E("Fortnite  ").decrypt());

    MemoryModule->HookProcess(E("FortniteClient-Win64-Shipping.exe").decrypt());
    //Memory::process_id = Memory::find_process(E("notepad.exe").decrypt());
    MemoryModule->SetupCommunication();
    Cache::BaseAddress = MemoryModule->GetPageAddress();
    ConsoleUtils::Write(E("Base Address : 0x").decrypt());
    std::cout << std::hex << Cache::BaseAddress << std::endl;
    ConsoleUtils::Write(E("Handle Address : 0x").decrypt());
    std::cout << std::hex << DeviceHandle << std::endl;
    ConsoleUtils::Write(E("Process Address : 0x").decrypt());
    std::cout << std::hex << ProcessId << std::endl;
    EraseMemory();

    ConsoleUtils::WriteLine(E("start OBS...").decrypt());

    while (true)
    {
        DWORD nigger = MemoryModule->GetProcessId(E("obs64.exe").decrypt());
        if (nigger)
            break;
    }

    Imported::Init();
    Overlay::StartOverlay();
}
