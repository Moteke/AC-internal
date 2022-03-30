#pragma once
#include <Windows.h>

class Imports {
	using wglSwapBuffers_t = int(__stdcall)(HDC handle);
	using SDL_ShowCursor_t = int(__cdecl)(int);
	using SDL_SetRelativeMouseMode_t = int(__cdecl)(bool);
	using SDL_SetWindowGrab_t = int(__cdecl)(void*, bool);

	bool initialized{ false };
	HMODULE sdl2;
	HMODULE opengl32;
public: 
	Imports();
	bool initialize();
	wglSwapBuffers_t* oWglSwapBuffers{ nullptr };
	SDL_ShowCursor_t* SDL_ShowCursor{ nullptr };
	SDL_SetRelativeMouseMode_t* SDL_SetRelativeMouseMode{ nullptr };
	SDL_SetWindowGrab_t* SDL_SetWindowGrab{ nullptr };
	wglSwapBuffers_t* wglSwapBuffers{ nullptr };
};