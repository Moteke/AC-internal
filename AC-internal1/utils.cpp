#include "utils.h"

DWORD utils::getBase() {
    DWORD newBase;
    // DWORD base = (DWORD)GetModuleHandle(NULL); <- slower but safer?
    // code below may not work in the future
    __asm {
        MOV EAX, DWORD PTR FS : [0x30]
        MOV EAX, DWORD PTR DS : [EAX + 0x8]
        MOV newBase, EAX
    }
    return newBase;
}

void utils::hookWithJump(DWORD hookAt, DWORD newFunc, int size) {
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

DWORD utils::trampolineSetup(DWORD hookedFn, int stolenBytesNum, DWORD hookFn) {
	// reserve memory for the trampoline
	DWORD trampoline = (DWORD)VirtualAlloc(0, stolenBytesNum + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (trampoline == 0) return 0;

	// setup the trampoline
	memcpy((void*)trampoline, (void*)hookedFn, stolenBytesNum);
	writeMemory<BYTE>(trampoline + stolenBytesNum, 0xE9);
	DWORD offset = hookedFn - (trampoline + 5);
	writeMemory<DWORD>(trampoline + stolenBytesNum + 1, offset);

	// set the hook in the hooked function
	hookWithJump(hookedFn, hookFn, stolenBytesNum);

	return trampoline;
}

float utils::distanceBetween(Vec3<float> first, Vec3<float> second) {
	return std::sqrt((first.x - second.x) * (first.x - second.x) + (first.y - second.y) * (first.y - second.y) + (first.z - second.z) * (first.z - second.z));
}

bool utils::w2s_c(Matrix4x4<float> projection, Vec3<float> position, Point<float>& screen, int* viewport) {
	// assume projection matrix is in column-major order
	float x = projection.elements[0] * position.x + projection.elements[4] * position.y + projection.elements[8] * position.z + projection.elements[12];
	float y = projection.elements[1] * position.x + projection.elements[5] * position.y + projection.elements[9] * position.z + projection.elements[13];
	float w = projection.elements[3] * position.x + projection.elements[7] * position.y + projection.elements[11] * position.z + projection.elements[15];

	if (w < 0.1f) return false;

	x = x / w;
	y = y / w;

	int width = viewport[2];
	int height = viewport[3];

	screen.x = x * (width / 2.0f) + (viewport[0] + (width / 2.0f));
	screen.y = y * (height / 2.0f) + (viewport[1] + (height / 2.0f));

	return true;
}