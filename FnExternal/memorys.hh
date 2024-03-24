#pragma once
#include <Windows.h>
#include <TlHelp32.h>

#define ReadWriteProcessMemory CTL_CODE(FILE_DEVICE_UNKNOWN, 0x71, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define ReturnProcessBaseAddress CTL_CODE(FILE_DEVICE_UNKNOWN, 0x72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SecurityToken 0x85b3e12

INT32 ProcessId;
uintptr_t PageAddress;
HANDLE DeviceHandle;

class Structs {
public:
	typedef struct _rw {
		INT32 process_id;
		ULONGLONG address;
		ULONGLONG buffer;
		ULONGLONG size;
		BOOLEAN write;
	} rw, * prw;

	typedef struct _ba {
		INT32 process_id;
		ULONGLONG* address;
	} ba, * pba;
private:
};

class Memory {
public:

	bool SetupCommunication() {
		DeviceHandle = CreateFileW((L"\\\\.\\\\{019237847183893-187826146798791-8167357189463-8722534623721}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!DeviceHandle || (DeviceHandle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}
	bool HookProcess(LPCTSTR ProcessName) {
		INT32 Process = GetProcessId(ProcessName);
		ProcessId = Process;
		if (Process)
			return true;
		return false;
	}

	void ReadProcessMemory(PVOID address, PVOID buffer, DWORD size) {
		Structs::_rw arguments = { 0 };

		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = ProcessId;
		arguments.write = FALSE;

		DeviceIoControl(DeviceHandle, ReadWriteProcessMemory, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void WriteProcessMemory(PVOID address, PVOID buffer, DWORD size) {
		Structs::_rw arguments = { 0 };

		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = ProcessId;
		arguments.write = TRUE;

		DeviceIoControl(DeviceHandle, ReadWriteProcessMemory, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t GetPageAddress() {
		uintptr_t image_address = { NULL };
		Structs::_ba arguments = { NULL };

		arguments.process_id = ProcessId;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(DeviceHandle, ReturnProcessBaseAddress, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	INT32 GetProcessId(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					ProcessId = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}
};

Memory* MemoryModule = new Memory();
