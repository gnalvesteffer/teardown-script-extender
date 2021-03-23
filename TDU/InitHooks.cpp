#include "Hooks.h"
#include "Logger.h"
#include "Globals.h"

void Hooks::initHooks()
{
	Globals::gModule = GetModuleHandle(NULL);
	WriteLog("teardown.exe: 0x%p", Globals::gModule);

	initRenderHooks();
	initLoggerHook();
	initLuaHooks();

	Hooks::InputHooks::hookCursor();
	Hooks::PlayerHooks::initPlayerHooks();
}