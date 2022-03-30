#pragma once

#include "EntManager.h"
#include "config.h"
#include "Drafter.h"
#include "ByteReplacer.h"
#include "Matrix.h"
#include "utils.h"
#include "mem.h"
#include <iostream>


class Juice {
	EntManager* manager;
	Drafter* drafter;

public:
	void doWork();
	Juice(EntManager* e, Drafter* d);
private:
	void mHealth();
	void mArmor();
	void mGranades();
	void mCarabineAmmo();
	void mNoClip();
	void ESP();
	void perEnemyESP(Player* enemy, Matrix4x4<float>* modelViewMatrix, int* viewport);

	ByteReplacer noClip1;
	ByteReplacer noClip2;
};