# AC-internal - internal cheat to [AssaultCube 1.3.0.1](https://drive.google.com/file/d/1AQOHXvdq4A0rBO_TYrbWzuHYO4mA1Aag/view?usp=sharing).
## Usage
1. Add the main directory to the Include Directories in the VS project.
2. Add imgui directory to Additional Include Directories.
3. Set Precompiled Headers to Not Using.
4. For the linker, add OpenGL32.lib to Additional Dependencies.
5. Compile the project for x86 and inject the DLL to the game linked above. Open the menu with INSERT.
