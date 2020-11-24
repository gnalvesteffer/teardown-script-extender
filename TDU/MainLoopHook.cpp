#include "Hooks.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "Teardown.h"

#include <detours.h>

typedef void	(*tMainLoop)			(Game* game, void* a2);

tMainLoop oMainLoop;

void hMainLoop(Game* game, void* a2)
{
	Teardown::pGame = game;
	oMainLoop(game, a2);

	Teardown::pGame->isQuickLoading = true;
	Teardown::pGame->statusTransition = gameStatus::playing;
	//Teardown::GetFunctionAddresses();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oMainLoop, hMainLoop);
	DetourTransactionCommit();
}

void Hooks::initMLHook()
{
	DWORD64 dwMainLoop = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx", Globals::gModule);
	dwMainLoop = Memory::readPtr(dwMainLoop, 1);

	oMainLoop = (tMainLoop)dwMainLoop;

	WriteLog("MainLoop: 0x%p | MainLoop hook: 0x%p", oMainLoop, hMainLoop);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oMainLoop, hMainLoop);
	DetourTransactionCommit();
}