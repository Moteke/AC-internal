#define _USE_MATH_DEFINES
#include "loop.h"

#include <cmath>

void Loop::loop()
{
	mHealth();
	mArmor();
	mGranades();
	mAmmo();
	mNoClip();
	ESP();

	// aimbot temp
	/*if (manager->getNumberOfEnemiesSP() < 1 || !g->player.granadeHack) return;
	DWORD base = utils::getBase();
	Player** enemyPtr = readMemory<Player**>(base + offsets::enemiesSinglePlayer);

	Player* firstEnemy = *enemyPtr;
	if (firstEnemy->health <= 0) return;

	Vec3<float> enemyPos = firstEnemy->positionHead;
	Vec3<float> ourPos = manager->getPlayer()->positionHead;

	Vec3<float> viewVec{ enemyPos.x - ourPos.x, enemyPos.y - ourPos.y, enemyPos.z - ourPos.z };
	float hyp = sqrt(viewVec.x * viewVec.x + viewVec.y * viewVec.y);

	float xy = -atan2f(viewVec.x, viewVec.y) * (180.0f / M_PI) + 180.0f;
	float z = atan2f(viewVec.z, hyp) * (180.0f / M_PI);

	const Vec2<float> myCursor = manager->getPlayer()->cursorXY;
	int smooth = 1.0;

	xy = (xy - myCursor.x) / smooth;
	z = (z - myCursor.y) / smooth;
	
	std::cout << "SETTING XY: " << xy << ", Z: " << z << '\n';

	manager->getPlayer()->cursorXY = { myCursor.x + xy, myCursor.y + z };*/
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
	if (!g->esp.enabled) return;
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

