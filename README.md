# Teardown Unleashed
TDU is an open source internal "tool" for testing / developing Teardown stuff, it should be a decent enough reference if you want to make your own modloader with lua bindings, or just your own mod in general.

## Key binds
* Insert to open the menu
* N to toggle noclip
* (While noclipping) Shift to go fast, Ctrl to go slow

## Features
* Lua executor **(slightly unstable at the moment)**, ready to implement custom functions with a couple already implemented ones (IsKeyDown, SetPlayerPos, SetPlayerVelocity).
* A loadbuffer hook, if you want to replace the game's scripts with your own, without the need of replacing the actual files.
* Noclip, for messing around and exploring the maps.
* Reversed and hooked GSLS bindings, so you can change any shader parameter you want.
* A couple debug features that were in the original debug menu (Render target, draw bounds, draw bodies).
* Reversed entity classes, although, i suggest you get those [here](https://github.com/SK83RJOSH/Teardown).
* A "cheat" menu, for debugging purposes, with features like god, no recoil, FOV changer, and a couple other settings for renderer related stuff

## Installation
Grab the opengl32.dll from releases, and paste it on your game's root directory (where teardown.exe is located)

## Build requirements
* [MS Detours](https://github.com/microsoft/Detours/releases)
* [GLEW](http://glew.sourceforge.net/)

## Important
* Make sure to change the output directory, and the debug working directory / command to the game's folder and exe
This only works for the latest version of the game (currently **0.4.6**)
* If you got any questions you can contact me at *Nahu#0331*
