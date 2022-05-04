#include "imports.h"
#include <iostream>

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
    if (sdl2 == NULL) {
        std::cout << "FATAL: SDL2.DLL NO HANDLE" << '\n';
        return false;
    }
    
    if (SDL_ShowCursor = (SDL_ShowCursor_t*)GetProcAddress(sdl2, "SDL_ShowCursor"), SDL_ShowCursor == NULL) {
        std::cout << "ERROR: SDL_ShowCursor() NO HANDLE" << '\n';
        return false;
    }

    if (SDL_SetRelativeMouseMode = (SDL_SetRelativeMouseMode_t*)GetProcAddress(sdl2, "SDL_SetRelativeMouseMode"), SDL_SetRelativeMouseMode == NULL) {
        std::cout << "ERROR: SDL_SetRelativeMouseMode() NO HANDLE" << '\n';
        return false;
    }
    
    if (SDL_SetWindowGrab = (SDL_SetWindowGrab_t*)GetProcAddress(sdl2, "SDL_SetWindowGrab"), SDL_SetWindowGrab == NULL) {
        std::cout << "ERROR: SDL_SetWindowGrab() NO HANDLE" << '\n';
        return false;
    }
    
    if (wglSwapBuffers = (wglSwapBuffers_t*)GetProcAddress(opengl32, "wglSwapBuffers"), wglSwapBuffers == NULL) {
        std::cout << "ERROR: wglSwapBuffers() NO HANDLE" << '\n';
        return false;
    }

    return true;
}
