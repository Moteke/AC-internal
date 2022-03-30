// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include "config.h"
#include "mem.h"
#include "Menu.h"
#include "Imports.h"
#include "Player.h"
#include "EntManager.h"
#include "utils.h"
#include "Point.h"
#include "Juice.h"
#include "Matrix.h"
#include "Drafter.h"
#include "Hook.h"

#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <cmath>

using wglSwapBuffers_t = int(__stdcall)(HDC handle);
static wglSwapBuffers_t* oWglSwapBuffers;

static Menu* menu;
static Imports* imports;
static Juice* juice;
static EntManager* manager;
static Drafter* drafter;

static int g_winWidth = 0;
static int g_winHeight = 0;
static HWND g_window;

void __stdcall doSomeGraphicsStuff(HDC handle) {
    drafter->initialize(handle);
    manager->updateConfig();
    menu->render();
    juice->doWork();

    // handle menu capturing
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        config::menuOpen = !config::menuOpen;

        if (config::menuOpen) {
            imports->SDL_ShowCursor(1);
            DWORD window = *(DWORD*)(utils::getBase() + 0x182884);
            imports->SDL_SetWindowGrab((void*)window, 0);
            imports->SDL_SetRelativeMouseMode(0);
			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);

            SetCursorPos(viewport[2] / 2, viewport[3] / 2);
        }
        else {
            imports->SDL_ShowCursor(0);
            DWORD window = *(DWORD*)(utils::getBase() + 0x182884);
            imports->SDL_SetWindowGrab((void*)window, 1);
            imports->SDL_SetRelativeMouseMode(1);
        }
    }
    if (GetAsyncKeyState(VK_DELETE) & 1) {
        config::closeMe = true;

        if (config::menuOpen) {
            config::menuOpen = false;

			imports->SDL_ShowCursor(0);
			DWORD window = *(DWORD*)(utils::getBase() + 0x182884);
			imports->SDL_SetWindowGrab((void*)window, 1);
			imports->SDL_SetRelativeMouseMode(1);
        }
    }

    oWglSwapBuffers(handle);
}

DWORD WINAPI partyStart(LPVOID lpParam) {
    bool allocated = AllocConsole() != 0;
    if (allocated) {
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
        std::cout << "DEBUG CONSOLE ATTACHED\n";
    }

    menu = new Menu();
    menu->initialize(L"AssaultCube");

    imports = new Imports();
    imports->initialize();

    manager = new EntManager();
    drafter = new Drafter();
    juice = new Juice(manager, drafter);

    g_window = FindWindow(NULL, L"AssaultCube");

    Hook swapBuffers{ (DWORD)imports->wglSwapBuffers, (DWORD)(&doSomeGraphicsStuff), 5 };
    oWglSwapBuffers = (wglSwapBuffers_t*)swapBuffers.getTrampoline();
    swapBuffers.enable();

    //oWglSwapBuffers = (wglSwapBuffers_t*)utils::trampolineSetup((DWORD)imports->wglSwapBuffers, 5, (DWORD)(&doSomeGraphicsStuff));

    while (!config::closeMe) {
        Sleep(1);
    }

    // cleanup
    swapBuffers.disable();
   /* config::menuOpen = false;*/

    Sleep(100); // wait so that we make sure that our swapbuffers hook is not working anymore
    delete menu;
    delete juice;
    delete drafter;
    delete manager;
    delete imports;

    fclose(stdout);
    FreeConsole();
    FreeLibraryAndExitThread((HMODULE)lpParam, 0);

    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, 0, &partyStart, hModule, 0, NULL);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

