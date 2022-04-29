#pragma once

#include "utils/utils.h"
#include "game/CEnemiesInfo.h"
#include "game/CPlayer.h"
#include <Windows.h>

class Offsets;

inline Offsets* g_offsets{};

class Offsets {
private:
	struct info {
		bool* connectedToServer = (bool*)0x187BD4;
	};

	struct view {
		DWORD matrix = 0x17AFE0;
	};

	struct enemies {
		CEnemiesInfo* multiplayer = (CEnemiesInfo*)0x187C10;
		CEnemiesInfo* singleplayer = (CEnemiesInfo*)0x18EFDC;
	};
public:

	info info{};
	view view{};
	enemies enemies{};

	CPlayer* player = (CPlayer*) 0x187C0C;

	Offsets() {

		DWORD base = utils::getBase();
		info.connectedToServer = (bool*)((DWORD)info.connectedToServer + base);
		view.matrix = view.matrix + base;
		enemies.multiplayer = (CEnemiesInfo*)((DWORD)enemies.multiplayer + base);
		enemies.singleplayer = (CEnemiesInfo*)((DWORD)enemies.singleplayer + base);
		player = *(CPlayer**)((DWORD)player + base);

		g_offsets = this;
	}

	~Offsets() {
		g_offsets = nullptr;
	}
};