#include "EntManager.h"


Player* EntManager::getPlayer()
{
	return readMemory<Player*>(base + offsets::player);
}

bool EntManager::isConnectedToServer()
{
	return readMemory<BYTE>(base + offsets::connectedToServer);
}

int EntManager::getNumberOfEnemiesSP()
{
	return readMemory<int>(base + offsets::enemiesSinglePlayer + 8);
}

Player** EntManager::getEnemiesSP()
{
	return readMemory<Player**>(base + offsets::enemiesSinglePlayer);
}

void EntManager::updateConfig()
{
	config::isMultiplayer = isConnectedToServer();
}

EntManager::EntManager() {
	base = utils::getBase();
}