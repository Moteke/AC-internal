#pragma once

#include "game/CPlayer.h"

class Aimbot {
public:
	static void aimAtPlayer(CPlayer* player);
	static CPlayer* getClosestToCursor();
	static Vec2<float> getAngleTo(Vec3<float> pos);
	static bool isValidTarget(CPlayer* player);
};