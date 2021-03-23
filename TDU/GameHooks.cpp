#include "Hooks.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "Teardown.h"

#include <detours.h>

typedef Entity*	(*tEntityCctor)			(void* mAlloc, Entity* parent);
tEntityCctor	newScript;

typedef void	(*tMainLoop)			(Game* game, void* a2);
tMainLoop MainLoop;

typedef Vox* (*tCreateVox)				(void* a1, void* a2, void* a3, void* a4, void* a5);
tCreateVox CreateVox;

Vox* hCreateVox(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	return CreateVox(a1, a2, a3, a4, a5);
}

void hMainLoop(Game* game, void* a2)
{
	Teardown::pGame = game;
	MainLoop(game, a2);

	Teardown::pGame->statusTransition = Teardown::gameStatus::menu;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)MainLoop, hMainLoop);
	DetourTransactionCommit();
}

void Hooks::initGameHooks()
{ 
	DWORD64 dwMainLoop = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx", Globals::gModule);
	MainLoop = (tMainLoop)Memory::readPtr(dwMainLoop, 1);

	DWORD64 dwNewScript = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\xD8\x33\xFF", "x????xxxxx", Globals::gModule);
	newScript = (tEntityCctor)Memory::readPtr(dwNewScript, 1);

	DWORD64 dwCreateVox = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x00\x84\xC0", "x????xxxx?xx", Globals::gModule);
	CreateVox = (tCreateVox)Memory::readPtr(dwNewScript, 1);

	WriteLog("MainLoop: 0x%p | hook: 0x%p", MainLoop, hMainLoop);
	WriteLog("CreateVox: 0x%p", CreateVox);
	WriteLog("Script::Script: 0x%p", newScript);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)MainLoop, hMainLoop);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)CreateVox, hCreateVox);
	DetourTransactionCommit();
}