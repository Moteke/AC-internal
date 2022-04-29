#include "enemiesService.h"

enemiesService::enemiesService()
{
	g_enemiesService = this;
}

enemiesService::~enemiesService()
{
	g_enemiesService = nullptr;
}

std::vector<CPlayer*> enemiesService::getEnemies()
{
	CEnemiesInfo* enemiesInfo{ g_offsets->enemies.singleplayer };

	if (*g_offsets->info.connectedToServer) {
		enemiesInfo = g_offsets->enemies.multiplayer;
	}

	std::vector<CPlayer*> result{};
	auto enemiesArr = *enemiesInfo->enemiesPtr;

	for (int i = 0; i < enemiesInfo->enemiesInGame; ++i) {
		auto x = *enemiesArr;
		if (x != 0) {
			if (x->vTable == 0x54B014 || x->vTable == 0x54AFEC) result.emplace_back(x);
		}
		++enemiesArr;
	}

	return result;
}

