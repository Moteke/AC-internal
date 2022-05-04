#include "Hook.h"

Hook::Hook(DWORD hookTarget, DWORD toHook, int bytesToSteal) 
	: hookTarget{hookTarget}, toHook{toHook}, bytesToSteal{bytesToSteal}
{
	trampoline = (DWORD)VirtualAlloc(0, bytesToSteal + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (trampoline != 0) {
		memcpy((void*)trampoline, (void*)hookTarget, bytesToSteal);
		writeMemory<BYTE>(trampoline + bytesToSteal, 0xE9);
		DWORD offset = hookTarget - (trampoline + 5);
		writeMemory<DWORD>(trampoline + bytesToSteal + 1, offset);

		trampolineSet = true;
	}

}

Hook::~Hook()
{
	disable();

	if(trampoline != 0)
		VirtualFree((LPVOID)trampoline, 0, MEM_RELEASE);
}

void Hook::enable()
{
	hookWithJump(hookTarget, toHook, bytesToSteal);
}

void Hook::disable()
{
	auto oldProtection = protectMemory<DWORD[3]>(hookTarget, PAGE_EXECUTE_READWRITE);
	memcpy((void*)hookTarget, (void*)trampoline, bytesToSteal);
	protectMemory<DWORD[3]>(hookTarget, oldProtection);
}

DWORD Hook::getTrampoline() const
{
	return trampoline;
}

void Hook::hookWithJump(DWORD hookAt, DWORD newFunc, int size) const
{
	if (size > 12) return;
	DWORD newOffset = newFunc - hookAt - 5;

	auto oldProtection = protectMemory<DWORD[3]>(hookAt, PAGE_EXECUTE_READWRITE);
	writeMemory<BYTE>(hookAt, 0xE9);
	writeMemory<DWORD>(hookAt + 1, newOffset);

	for (int i = 5; i < size; ++i) {
		writeMemory<BYTE>(hookAt + i, 0x90);
	}

	protectMemory<DWORD[3]>(hookAt, oldProtection);
}
