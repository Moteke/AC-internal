#pragma once
#include <Windows.h>

template<typename T>
T* pointMemory(DWORD adr) {
	return ((T*)adr);
}

template<typename T>
T readMemory(DWORD adr) {
	return *((T*)(adr));
}

template<typename T>
T writeMemory(DWORD adr, T val) {
	T answer = *((T*)(adr));
	*((T*)(adr)) = val;
	return answer;
}

template<typename T>
DWORD protectMemory(DWORD adr, DWORD newProtection) {
	DWORD oldProt = 0;
	VirtualProtect((LPVOID)adr, sizeof(T), newProtection, &oldProt);
	return oldProt;
}

DWORD protectMemory(DWORD adr, DWORD newProtection, int size);

template<int SIZE>
void writeNOP(DWORD address) {
	DWORD oldProt = protectMemory<BYTE[SIZE]>(address, PAGE_EXECUTE_READWRITE);
	for (int i = 0; i < SIZE; ++i)
		writeMemory<BYTE>(address + i, 0x90);
	protectMemory<BYTE[SIZE]>(address, oldProt);
}
