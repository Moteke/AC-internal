#pragma once

#include <Windows.h>

class ByteReplacer {
public:
	ByteReplacer(uintptr_t addr, int numOfBytesToOverwrite, BYTE* bytesToInsert, int numOfBytesToInsert);
	ByteReplacer() = default;
	~ByteReplacer();
	bool apply();
	bool remove();

	void setBytesToInsert(BYTE* t);
private:
	bool applied{ false };
	int numOfOriginalBytes;
	int numOfNewBytes;
	uintptr_t addr{ 0 };
	BYTE* toInsert{ nullptr };
	BYTE* stolenBytes{ nullptr };
};