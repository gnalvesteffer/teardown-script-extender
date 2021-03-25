#include "Hooks.h"
#include "Logger.h"

void Hooks::InitHooks()
{
	Hooks::BaseHooks::HookCW();
	Hooks::BaseHooks::HookMain();

	Hooks::GLHooks::HookGlewInit();
	Hooks::GLHooks::HookSwapBuffers();

	Hooks::PlayerHooks::HookUpdateCamera();
	Hooks::PlayerHooks::HookUpdateCollisions();

	Hooks::InputHooks::HookCursorPos();
	Hooks::MiscHooks::HookLogFunction();
	Hooks::LuaHooks::HookLoadBuffer();
}