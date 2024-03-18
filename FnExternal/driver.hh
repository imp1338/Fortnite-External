#pragma once
#include <TlHelp32.h>
#include "memorys.hh"

template <typename T>
T read(uint64_t address) {
	T buffer{ };
	MemoryModule->ReadProcessMemory((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {

	MemoryModule->WriteProcessMemory((PVOID)address, &buffer, sizeof(T));
	return buffer;
}