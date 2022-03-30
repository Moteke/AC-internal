#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Weapon.h"

#include <cstdint>

using Vector2 = Vec2<float>;
using Vector3 = Vec3<float>;

struct Player
{
public:
	char pad_0000[4]; //0x0000
	Vector3 positionHead; //0x0004
	char pad_0010[24]; //0x0010
	Vector3 position; //0x0028
	Vector2 cursorXY; //0x0034
	char pad_003C[56]; //0x003C
	int8_t movementUD; //0x0074
	int8_t movementLR; //0x0075
	int8_t movementType; //0x0076
	int8_t N0000046D; //0x0077
	char pad_0078[116]; //0x0078
	int32_t health; //0x00EC
	int32_t armor; //0x00F0
	char pad_00F4[112]; //0x00F4
	int32_t shootingState; //0x0164
	char pad_0168[157]; //0x0168
	char name[16]; //0x0205
	char pad_0215[247]; //0x0215
	int32_t team; //0x030C
	char pad_0310[52]; //0x0310
	Weapon* weaponCarbine; //0x0344
	Weapon* weaponShotgun; //0x0348
	Weapon* weaponSubgun; //0x034C
	Weapon* weaponSniper; //0x0350
	Weapon* weaponAssault; //0x0354
	Weapon* weaponGrenade; //0x0358
	Weapon* weaponAkimbo; //0x035C
	char pad_0360[4]; //0x0360
	Weapon* weaponActive; //0x0364
	char pad_0368[536]; //0x0368
}; 