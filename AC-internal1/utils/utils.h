#pragma once

#include "mem.h"
#include "Vec3.h"
#include "Matrix.h"
#include "Point.h"

#include <Windows.h>
#include <cmath>

namespace utils
{
    DWORD getBase();
	HWND getWindow();
	void hookWithJump(DWORD hookAt, DWORD newFunc, int size);
	DWORD trampolineSetup(DWORD hookedFn, int stolenBytesNum, DWORD hookFn);

	float distanceBetween(Vec3<float> first, Vec3<float> second);
	bool w2s_c(Matrix4x4<float> projection, Vec3<float> position, Point<float>& screen, int* viewport);
}