#define _USE_MATH_DEFINES
#include "loop.h"
#include "aimbot/aimbot.h"

#include <cmath>

void Loop::loop()
{
	mHealth();
	mArmor();
	mGranades();
	mAmmo();
	mNoClip();
	ESP();
	aimbot();

	if (!g->player.clipHack) return;

	// CODE BELOW FOR DEV ONLY 
	// TRYING TO DO THE 'ENEMY VISIBLE' CHECK

	/*DWORD isCrossing = 0x004C8450;

	auto enemies = g_enemiesService->getEnemies();
	for (auto enemy : enemies) {
		auto player = g_offsets->player;

		DWORD playerPos = (DWORD) (&(player->positionHead));
		DWORD enemyPos = (DWORD) (&(enemy->positionHead));

		DWORD result{ 0 };

		Vec3<float> feedback{};
		Vec3<float>* x = &feedback;

		__asm {
			push x;
			push playerPos;
			mov edx, enemyPos;
			mov ecx, enemy;
			call isCrossing;
			mov result, eax;
			add esp, 8;
		}

		std::cout << "ENEMY " << enemy->name;

		if (result != 0)  std::cout << " is visible!\n";
		else std::cout << " is NOT visible!\n";

		std::cout << "FEEDBACK: X: " << feedback.x << ", Y: " << feedback.y << ", Z: " << feedback.z << '\n';

		break;
	}*/

	
}

void Loop::mHealth()
{
	if (g->player.healthHack)
		g_offsets->player->health = g->player.setHealth;
}

void Loop::mArmor()
{
	if (g->player.armorHack)
		g_offsets->player->armor = g->player.setArmor;
}

void Loop::mGranades()
{
	//if (config::granadeHack);
		//manager->getPlayer()->grenades = 5;
}

void Loop::mAmmo()
{
	if (g->player.ammoHack)
		*(g_offsets->player->weaponActive->ammoPtr) = 30;
	
	if (g->player.clipHack)
		*(g_offsets->player->weaponActive->clipPtr) = 99;
}

void Loop::mNoClip()
{
	static ByteReplacer noClip1{ utils::getBase() + 0x7CAA5, 2, new BYTE[2]{ 0x90, 0x90 }, 2 };
	static ByteReplacer noClip2(utils::getBase() + 0x55E36, 2, new BYTE[2]{ 0x90, 0x90 }, 2);

	if (g->movement.noclipHack) {
		if(noClip1.apply() && noClip2.apply())
			g_offsets->player->movementType = 4;
	}
	else {
		if(noClip1.remove() && noClip2.remove())
			g_offsets->player->movementType = 0;
	}
}

void Loop::ESP()
{
	if (!g->esp.enabled || g_drafter == nullptr) return;
	DWORD base = utils::getBase();

	Matrix4x4<float>* modelView = (Matrix4x4<float>*)(g_offsets->view.matrix);

	GLint viewport[4];
	g_drafter->getViewport(viewport);

	if (!g_drafter->preCall()) return;

	for (CPlayer* enemy : g_enemiesService->getEnemies()) {
		perEnemyESP(enemy, modelView, viewport);
	}

	g_drafter->afterCall();
}

void Loop::perEnemyESP(CPlayer* enemy, Matrix4x4<float>* modelViewMatrix, int* viewport)
{
	Vec3<float> color = Vec3<float>(1.0f, 0.0f, 0.0f);

	if (g->esp.distinguishTeams && enemy->team == g_offsets->player->team) {
		color = Vec3<float>(0.0f, 1.0f, 0.0f);
	}

	if (enemy->health > 0) {
		Point<float> scr = Point(0.0f, 0.0f);
		Vec3 enemyPos = enemy->positionHead;
		enemyPos.z -= 2.0f; // center

		if (utils::w2s_c(*modelViewMatrix, enemyPos, scr, viewport)) {

			float distance = utils::distanceBetween(g_offsets->player->position, enemy->position);
			float scale = distance / 20.0f;

			float boxWidth = 100.0f / scale;
			float boxHeight = 200.0f / scale;
			g_drafter->drawOutlineAround(scr, boxWidth, boxHeight, color);

			if (g->esp.showHealthBar) {
				g_drafter->drawOutlineAround(Point{ scr.x + (boxWidth / 2) + (10.0f / scale), scr.y }, 8.0f / scale, boxHeight, color);
				g_drafter->drawRectangle(Point{ scr.x + (boxWidth / 2) + (6.5f / scale), scr.y - (boxHeight / 2) + boxHeight * min((enemy->health / 100.0f), 1.0f) },
					7.0f / scale,
					boxHeight - boxHeight * min(((100 - enemy->health) / 100.0f), 1.0f),
					Vec3<float>{0.0f, 1.0f, 0.0f});
			}

			if (g->esp.showNames) {
				Point bottomLeftCorner{ scr.x - boxWidth / 2, scr.y - boxHeight / 2 };
				bottomLeftCorner.y -= 20.0f;
				g_drafter->drawTextCentered(bottomLeftCorner, color, boxWidth, "%s", enemy->name);
			}
		}
	}
}

void Loop::aimbot()
{
	static CPlayer* currentTarget{nullptr};

	if (!g->aimbot.enabled || (g->aimbot.lockOnlyWhenShooting && !(GetKeyState(VK_LBUTTON) & 0x8000))) {
		currentTarget = nullptr;
		return;
	}
	
	if(!Aimbot::isValidTarget(currentTarget))
		currentTarget = Aimbot::getClosestToCursor();

	if (currentTarget == nullptr) return;

	Aimbot::aimAtPlayer(currentTarget);
}

