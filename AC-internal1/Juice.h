#pragma once

#include "config.h"
#include "drafter.h"
#include "byte_replacer.h"
#include "matrix.h"
#include "utils/utils.h"
#include "mem.h"
#include "game/CPlayer.h"
#include "services/enemiesService.h"
#include "offsets.h"
#include <iostream>


class Loop {
public:
	static void loop();
private:
	static void mHealth();
	static void mArmor();
	static void mGranades();
	static void mAmmo();
	static void mNoClip();
	static void ESP();
	static void perEnemyESP(CPlayer* enemy, Matrix4x4<float>* modelViewMatrix, int* viewport);
};