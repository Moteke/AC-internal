#pragma once

#include <cstdint>

struct Weapon {
	char pad_0000[12]; //0x0000
	char(*weaponNamePtr)[16]; //0x000C
	int32_t* clipPtr; //0x0010
	int32_t* ammoPtr; //0x0014
	char pad_0018[8]; //0x0018
	int32_t reloadState; //0x0020
};