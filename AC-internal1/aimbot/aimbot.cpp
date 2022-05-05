#define _USE_MATH_DEFINES

#include "aimbot.h"
#include "offsets.h"
#include "services/enemiesService.h"
#include "utils/Vec2.h"
#include "utils/Vec3.h"

#include <cmath>
#include <limits>
#include <iostream>

void Aimbot::aimAtPlayer(CPlayer* player)
{
	Vec2 angles{ getAngleTo(player->positionHead) };
	Vec2 playerCursor{ g_offsets->player->cursorXY };

	std::cout << "ANGLES X: " << angles.x - playerCursor.x << " Y: " << angles.y - playerCursor.y << '\n';

	playerCursor.x += angles.x - playerCursor.x;
	playerCursor.y += angles.y - playerCursor.y;

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

		if (newDistance < bestDistance && enemy->health > 0) {
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
