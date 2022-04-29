//#include "playerService.h"
//
//#include "utils/utils.h"
//#include "Offsets.h"
//
//playerService::playerService()
//{
//	player = *reinterpret_cast<CPlayer**>(utils::getBase() + offsets::player);
//
//	g_player = this;
//}
//
//playerService::~playerService()
//{
//	g_player = nullptr;
//}
//
//char* playerService::getName() const
//{
//	return player->name;
//}
//
//int playerService::getHealth() const
//{
//	return player->health;
//}
//
//void playerService::setHealth(int health)
//{
//	player->health = health;
//}
//
//int playerService::getArmor() const
//{
//	return player->armor;
//}
//
//int playerService::setArmor(int armor)
//{
//	player->armor = armor;
//}
//
//void playerService::setAmmo(int ammo)
//{
//	*player->weaponActive->ammoPtr = ammo;
//}
//
//void playerService::setClip(int clip)
//{
//	*player->weaponActive->clipPtr = clip;
//}
//
//
