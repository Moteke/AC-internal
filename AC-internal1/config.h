#pragma once

class Settings;

inline Settings* g{ };

class Settings {
	struct player {
		bool healthHack = false;
		int setHealth = 100;
		bool armorHack = false;
		int setArmor = 100;
		bool granadeHack = false;
		int setGranade = 5;
		bool ammoHack = false;
		int setAmmo = 90;
		bool clipHack = false;
		int setClip = 30;
	};

	struct movement {
		bool noclipHack = false;
	};

	struct esp {
		bool enabled = false;
		bool distinguishTeams = false;
		bool showNames = false;
		bool showHealthBar = false;
	};

	struct aimbot {
		bool enabled{ false };
		float lockDistance{ 200.0f };
		bool aimAtTeam{ false };
	};

	struct game {
		bool isMultiplayer = false;
	};

	struct menu {
		bool open = false;
		bool showDemo = false;
		bool showAnother = false;
	};

public:
	player player{};
	movement movement{};
	esp esp{};
	aimbot aimbot{};
	game game{};
	menu menu{};

	bool unload = false;

	Settings() {
		g = this;
	}

	~Settings() {
		g = nullptr;
	}
};

