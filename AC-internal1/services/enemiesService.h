#pragma once

#include "game/CEnemiesInfo.h"
#include "game/CPlayer.h"
#include "Offsets.h"

#include <vector>

class enemiesService;

inline enemiesService* g_enemiesService{};

class enemiesService {
public:
	enemiesService();
	~enemiesService();

	std::vector<CPlayer*> getEnemies();
};