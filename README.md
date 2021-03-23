# Teardown Script Extender
Adds new Lua functions to Teardown's scripting API, allowing for greater modding potential.

Built on top of [Teardown Unleashed by nxhu64](https://github.com/nxhu64/TDU).

## Installation
Grab the opengl32 dll from releases, and paste it on your game's root directory (where Teardown.exe is located)

## Build requirements
* [MS Detours](https://github.com/microsoft/Detours)
* [GLEW](http://glew.sourceforge.net/)

## Building:
1. Clone the repository **`git clone --recurse-submodules https://github.com/ss-gnalvesteffer/teardown-script-extender`**
2. Open TDU.sln on Visual Studio (TDU was coded on VS 2019, there's no guarantee it'll work on any older versions without changing some stuff)
3. On project properties->Debugging set the Command argument to your version of Teardown (SteamStub has anti-debugging, you'll have to figure how to circumvent that on your own) and set the working directory to Teardown's root directory (where the exe is located)
4. On VC++ Directories set your include/library directories for GLEW and MS Detours
5. Everything should be ready to build.

## Important
* Make sure to change the output directory, and the debug working directory / command to the game's folder and exe

## Special thanks to
* [nxhu64](https://github.com/nxhu64) for helping me with this project and allowing me to use TDU as the foundation for this script extender.
Thank you so much for your help, knowledge, ideas and inspiration