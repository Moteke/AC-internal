#define _USE_MATH_DEFINES

#include "aimbot.h"
#include "config.h"
#include "offsets.h"
#include "services/enemiesService.h"
#include "utils/Vec2.h"
#include "utils/Vec3.h"

#include <cmath>
#include <limits>
#include <iostream>

void Aimbot::aimAtPlayer(CPlayer* player)
{
	Vec2 angles{ getAngleTo(player->positionHead)};
	Vec2 playerCursor{ g_offsets->player->cursorXY };

	auto x = (angles.x - playerCursor.x) / g->aimbot.smooth;
	auto y = (angles.y - playerCursor.y) / g->aimbot.smooth;

	playerCursor.x += x / g->aimbot.test;
	playerCursor.y += y / g->aimbot.test;

	g_offsets->player->cursorXY = playerCursor;
}

CPlayer* Aimbot::getClosestToCursor()
{
	// parenthesis to prevent the macro expansion
	double bestDistance{ (std::numeric_limits<double>::max)() };
	CPlayer* bestPlayer{ nullptr };

	const Vec3<float> playerHeadPos = g_offsets->player->positionHead;
	const Vec2 playerCursor{ g_offsets->player->cursorXY };

	auto enemies{ g_enemiesService->getEnemies() };
	for (auto enemy : enemies) {
		auto angles{ getAngleTo(enemy->positionHead) };

		double x = angles.x - playerCursor.x;
		double y = angles.y - playerCursor.y;
		double newDistance = x * x + y * y;

		if (newDistance < bestDistance && isValidTarget(enemy)) {
			bestDistance = newDistance;
			bestPlayer = enemy;
		}
	}

	return bestPlayer;
}

Vec2<float> Aimbot::getAngleTo(Vec3<float> pos)
{
	const Vec3<float> playerHeadPos = g_offsets->player->positionHead;
	const Vec3<float> viewVec{ pos.x - playerHeadPos.x, pos.y - playerHeadPos.y, pos.z - playerHeadPos.z };

	float hyp = sqrt(viewVec.x * viewVec.x + viewVec.y * viewVec.y);
	float x = -atan2f(viewVec.x, viewVec.y) * (180.0f / M_PI) + 180.0f;
	float y = atan2f(viewVec.z, hyp) * (180.0f / M_PI);

	return Vec2<float>{x, y};
}

bool Aimbot::isValidTarget(CPlayer* player)
{
	if (player == nullptr) return false;
	if (utils::distanceBetween(player->position, g_offsets->player->position) > g->aimbot.lockDistance) return false;
	if (!g->aimbot.aimAtTeam && g_offsets->player->team == player->team) return false;

	return player->health > 0;
}
