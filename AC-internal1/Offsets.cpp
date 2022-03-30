#include "Offsets.h"

namespace offsets
{
	DWORD player = 0x187C0C;

	/*
		Entities
		Size of array of ptrs to enemies:	enemies + 4
		Number of enemies:					enemies + 8
	*/
	DWORD enemiesSinglePlayer = 0x18EFDC;
	DWORD enemiesMultiPlayer = 0x187C10;

	DWORD connectedToServer = 0x187BD4;

	DWORD modelViewMatrix = 0x17AFE0;
}