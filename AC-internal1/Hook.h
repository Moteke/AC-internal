#pragma once

#include "mem.h"

#include <Windows.h>

class Hook {
public:
	Hook(DWORD hookTarget, DWORD toHook, int bytesToSteal);
	~Hook();
	void enable();
	void disable();
	DWORD getTrampoline();
private:
	void hookWithJump(DWORD hookAt, DWORD newFunc, int size);

	DWORD trampoline{ 0 };
	DWORD hookTarget{ 0 };
	DWORD toHook{ 0 };
	int bytesToSteal{ 0 };

	bool trampolineSet{ false };
};