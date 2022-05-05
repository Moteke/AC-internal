#pragma once

#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_win32.h"

class Menu {
public:
	bool initialize();
	void render();
	void showSingleplayerWindow();
	void showESPWindow();
	void showAimbotWindow();
	~Menu();
private:
	
private:
	ImVec4 clear_color{ ImVec4(0.45f, 0.55f, 0.60f, 1.00f) };
	bool showingSingleplayerWindow{ false };
	bool showingESPWindow{ false };
	bool showingAimbotWindow{ false };
	HWND window{ NULL };
};