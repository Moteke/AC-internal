#pragma once

#include "CPlayer.h"
#include <cstdint>

class CEnemiesInfo
{
public:
	CPlayer* (*enemiesPtr)[32]; //0x0000
	int32_t enemiesArrSize; //0x0004
	int32_t enemiesInGame; //0x0008
};