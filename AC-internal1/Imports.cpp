#include "imports.h"

Imports::Imports()
{
    sdl2 = GetModuleHandle(L"SDL2.DLL");
    opengl32 = GetModuleHandle(L"OpenGL32.DLL");
}

bool Imports::initialize()
{
    /*
        Need to refactor the 'return false' problems
        into proper debug console errors
    */
    if (sdl2 == NULL) return false;

    SDL_ShowCursor = (SDL_ShowCursor_t*)GetProcAddress(sdl2, "SDL_ShowCursor");
    if (SDL_ShowCursor == NULL) return false;

    SDL_SetRelativeMouseMode = (SDL_SetRelativeMouseMode_t*)GetProcAddress(sdl2, "SDL_SetRelativeMouseMode");
    if (SDL_SetRelativeMouseMode == NULL) return false;

    SDL_SetWindowGrab = (SDL_SetWindowGrab_t*)GetProcAddress(sdl2, "SDL_SetWindowGrab");
    if (SDL_SetWindowGrab == NULL) return false;

    wglSwapBuffers = (wglSwapBuffers_t*)GetProcAddress(opengl32, "wglSwapBuffers");
    if (wglSwapBuffers == NULL) return false;

    return true;
}
