#include "mem.h"

DWORD protectMemory(DWORD adr, DWORD newProtection, int size) {
	DWORD oldProt = 0;
	VirtualProtect((LPVOID)adr, size, newProtection, &oldProt);
	return oldProt;
}