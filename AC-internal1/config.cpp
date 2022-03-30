#include "config.h"
namespace config
{
	bool initialized = false;
	bool show_demo_window = false;
	bool show_another_window = false;
	
	/*
		PLAYER
	*/
	bool healthHack = false;
	int healthToSet = 100;
	bool armorHack = false;
	int armorToSet = 50;
	bool granadeHack = false;
	int granadeToSet = 1;
	bool carabineAmmoHack = false;
	int carabineAmmoToSet = 30;

	bool menuOpen = false;

	bool noclipHack = false;

	bool espHack = false;
	bool esp_distinguishTeams = false;
	bool esp_showNames = false;
	bool esp_showHealthBar = false;

	bool isMultiplayer = false;

	bool closeMe = false;
}
