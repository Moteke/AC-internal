#define _USE_MATH_DEFINES
#include "Juice.h"

#include <cmath>

void Juice::doWork()
{
	mHealth();
	mArmor();
	mGranades();
	mCarabineAmmo();
	mNoClip();
	ESP();

	// aimbot temp
	if (manager->getNumberOfEnemiesSP() < 1 || !config::granadeHack) return;
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

	manager->getPlayer()->cursorXY = { myCursor.x + xy, myCursor.y + z };
	//manager->getPlayer()->cursorXY = { xy, z };
}

Juice::Juice(EntManager* e, Drafter* d)
	: noClip1(utils::getBase() + 0x7CAA5, 2, nullptr, 2),
	  noClip2(utils::getBase() + 0x55E36, 2, nullptr, 2),
	  manager(e),
	  drafter(d)
{
	{
		BYTE* a = new BYTE[2]{ 0x90, 0x90 };
		noClip1.setBytesToInsert(a);
	}
	{
		BYTE* a = new BYTE[2]{ 0x90, 0x90 };
		noClip2.setBytesToInsert(a);
	}
}

void Juice::mHealth()
{
	if (config::healthHack)
		manager->getPlayer()->health = config::healthToSet;
}

void Juice::mArmor()
{
	if (config::armorHack) 
		manager->getPlayer()->armor = config::armorToSet;
}

void Juice::mGranades()
{
	if (config::granadeHack);
		//manager->getPlayer()->grenades = 5;
}

void Juice::mCarabineAmmo()
{
	if (config::carabineAmmoHack)
		*(manager->getPlayer()->weaponActive->ammoPtr) = config::carabineAmmoToSet;
}

void Juice::mNoClip()
{
	if (config::noclipHack) {
		if(noClip1.apply() && noClip2.apply())
			manager->getPlayer()->movementType = 4;
	}
	else {
		if(noClip1.remove() && noClip2.remove())
			manager->getPlayer()->movementType = 0;
	}
}

void Juice::ESP()
{
	if (!config::espHack) return;
	DWORD base = utils::getBase();

	Matrix4x4<float>* modelView = (Matrix4x4<float>*)(base + offsets::modelViewMatrix);
	int playerTeam = manager->getPlayer()->team;

	GLint viewport[4];
	drafter->getViewport(viewport);

	if (!drafter->preCall()) return;

	if (!manager->isConnectedToServer()) {
		Player** enemyPtr = readMemory<Player**>(base + offsets::enemiesSinglePlayer);

		// singleplayer
		for (int i = 0; i < manager->getNumberOfEnemiesSP(); ++i) {
			perEnemyESP(*enemyPtr, modelView, viewport);
			enemyPtr++;
		}
	}
	else {
		Player** enemyPtr = readMemory<Player**>(base + offsets::enemiesMultiPlayer);

		if ((DWORD)enemyPtr > base) {
			++enemyPtr; // skip first element which is always 0

			// multiplayer
			for (int i = 1; i < readMemory<int>(base + offsets::enemiesMultiPlayer + 8); ++i) {
				Player* e = *enemyPtr;

				if (e != 0) {
					DWORD test = readMemory<DWORD>((DWORD)e);
					if (test == 0x54AFEC || test == 0x54B014) perEnemyESP(e, modelView, viewport);
				}

				enemyPtr++;
			}
		}
	}

	drafter->afterCall();
}

void Juice::perEnemyESP(Player* enemy, Matrix4x4<float>* modelViewMatrix, int* viewport)
{
	Vec3<float> color = Vec3<float>(1.0f, 0.0f, 0.0f);

	if (config::esp_distinguishTeams && enemy->team == manager->getPlayer()->team) {
		color = Vec3<float>(0.0f, 1.0f, 0.0f);
	}

	if (enemy->health > 0) {
		Point<float> scr = Point(0.0f, 0.0f);
		Vec3 enemyPos = enemy->positionHead;
		enemyPos.z -= 2.0f; // center

		if (utils::w2s_c(*modelViewMatrix, enemyPos, scr, viewport)) {
			float distance = utils::distanceBetween(manager->getPlayer()->position, enemy->position);
			float scale = distance / 20.0f;

			float boxWidth = 100.0f / scale;
			float boxHeight = 200.0f / scale;
			drafter->drawOutlineAround(scr, boxWidth, boxHeight, color);

			if (config::esp_showHealthBar) {
				drafter->drawOutlineAround(Point{ scr.x + (boxWidth / 2) + (10.0f / scale), scr.y }, 8.0f / scale, boxHeight, color);
				drafter->drawRectangle(Point{ scr.x + (boxWidth / 2) + (6.5f / scale), scr.y - (boxHeight / 2) + boxHeight * min((enemy->health / 100.0f), 1.0f) },
					7.0f / scale,
					boxHeight - boxHeight * min(((100 - enemy->health) / 100.0f), 1.0f),
					Vec3<float>{0.0f, 1.0f, 0.0f});
			}

			if (config::esp_showNames) {
				Point bottomLeftCorner{ scr.x - boxWidth / 2, scr.y - boxHeight / 2 };
				bottomLeftCorner.y -= 20.0f;
				drafter->drawTextCentered(bottomLeftCorner, color, boxWidth, "%s", enemy->name);
			}
		}
	}
}

