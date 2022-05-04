// dllmain.cpp : Defines the entry point for the DLL application.

#include "config.h"
#include "mem.h"
#include "Menu.h"
#include "Imports.h"
#include "utils/utils.h"
#include "loop.h"
#include "Matrix.h"
#include "Drafter.h"
#include "Hook.h"
#include "Offsets.h"
#include "services/enemiesService.h"

#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <cmath>

using wglSwapBuffers_t = int(__stdcall)(HDC handle);
static wglSwapBuffers_t* oWglSwapBuffers;

static std::unique_ptr<Menu> menu;
static Imports* imports;
static Drafter* drafterInstance;

static int g_winWidth = 0;
static int g_winHeight = 0;

void __stdcall doSomeGraphicsStuff(HDC handle) {

    if (g_drafter == nullptr) {
        delete drafterInstance;
        drafterInstance = new Drafter{};

        if (g_drafter == nullptr) {
            std::cout << "[FATAL] couldn't initialize Drafter object: g_drafter is nullptr" << '\n';
        }
    }

    menu->render();
    Loop::loop();

    // handle menu capturing
    if (GetAsyncKeyState(VK_INSERT) & 1) {
        g->menu.open = !g->menu.open;

        if (g->menu.open) {
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
        g->unload = true;

        if (g->menu.open) {
            g->menu.open = false;

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

    auto settingsInstance = std::make_unique<Settings>();
    auto offsetsInstance = std::make_unique<Offsets>();
    auto enemiesInstance = std::make_unique<enemiesService>();

    menu = std::make_unique<Menu>();
    menu->initialize();

    imports = new Imports();
    imports->initialize();

    Hook swapBuffers{ (DWORD)imports->wglSwapBuffers, (DWORD)(&doSomeGraphicsStuff), 5 };
    oWglSwapBuffers = (wglSwapBuffers_t*)swapBuffers.getTrampoline();
    swapBuffers.enable();

    //oWglSwapBuffers = (wglSwapBuffers_t*)utils::trampolineSetup((DWORD)imports->wglSwapBuffers, 5, (DWORD)(&doSomeGraphicsStuff));

    while (!g->unload) {
        Sleep(50);
    }

    // cleanup
    swapBuffers.disable();
   /* config::menuOpen = false;*/

    Sleep(100); // wait so that we make sure that our swapbuffers hook is not working anymore
    menu.reset();

    /*
        DrafterInstance is initialized in the swapbuffers hook
        because we need the drawing thread for that purpose. 
        However, we delete it here to clean up before closing.
    */
    delete drafterInstance;
    delete imports;

    enemiesInstance.reset();
    offsetsInstance.reset();
    settingsInstance.reset();

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

