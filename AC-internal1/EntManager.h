#pragma once

#include <Windows.h>
#include "Player.h"
#include "utils.h"
#include "config.h"
#include "mem.h"
#include "Offsets.h"

class EntManager {
public:
	EntManager();
	Player* getPlayer();
	bool isConnectedToServer();

	int getNumberOfEnemiesSP();
	Player** getEnemiesSP();

	void updateConfig();
private:
	DWORD base;
};