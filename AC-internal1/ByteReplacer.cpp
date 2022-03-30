#include "ByteReplacer.h"
#include "mem.h"

ByteReplacer::ByteReplacer(uintptr_t addr, int numOfBytesToOverwrite, BYTE* bytesToInsert, int numOfBytesToInsert)
	: toInsert{ bytesToInsert }, addr{ addr }, numOfOriginalBytes{ numOfBytesToOverwrite }, numOfNewBytes{ numOfBytesToInsert }
{
	auto oldProt = protectMemory(addr, PAGE_EXECUTE_READWRITE, numOfBytesToOverwrite);

	// we need original bytes to later reverse the hook
	stolenBytes = new BYTE[numOfBytesToOverwrite];
	memcpy(stolenBytes, reinterpret_cast<void*>(addr), numOfBytesToOverwrite);

	protectMemory(addr, oldProt, numOfBytesToOverwrite);
}

ByteReplacer::~ByteReplacer()
{
	delete[] toInsert;
	delete[] stolenBytes;
}

bool ByteReplacer::apply()
{
	if (applied) return false;

	auto oldProt = protectMemory(addr, PAGE_EXECUTE_READWRITE, numOfOriginalBytes);

	memmove(reinterpret_cast<void*>(addr), toInsert, numOfNewBytes);
	// make sure that extra bytes are filled with 0x90

	for (int i = 0; i < numOfOriginalBytes - numOfNewBytes; ++i) {
		writeMemory<BYTE>(addr + numOfNewBytes + i, 0x90);
	}

	protectMemory(addr, oldProt, numOfOriginalBytes);
	applied = true;

	return true;
}

bool ByteReplacer::remove()
{
	if (!applied) return false;

	auto oldProt = protectMemory(addr, PAGE_EXECUTE_READWRITE, numOfOriginalBytes);

	memmove(reinterpret_cast<void*>(addr), stolenBytes, numOfOriginalBytes);

	protectMemory(addr, oldProt, numOfOriginalBytes);
	applied = false;

	return true;
}

void ByteReplacer::setBytesToInsert(BYTE* t)
{
	toInsert = t;
}
