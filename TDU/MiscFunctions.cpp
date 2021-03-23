#include "Memory.h"
#include "Globals.h"
#include "Functions.h"
#include "Game.h"
#include "Logger.h"

#include <detours.h>

typedef void(*tLoadLevel) (Game* pGame, small_string* lvlName);
tLoadLevel LoadLevel;

void hLoadLevel(Game* pGame, small_string* lvlName)
{
	WriteLog("Loaded level: %s | %p", lvlName->c_str());
	LoadLevel(pGame, lvlName);
}


void Teardown::Functions::MiscFunctions::getFunctionAddresses()
{
	LoadLevel = (tLoadLevel)Memory::FindPattern((PBYTE)"\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\x48\x81\xC1\x00\x00\x00\x00", "xxxxxxxxxxxx????", Globals::gModule);

	WriteLog("LoadLevel: 0x%p | hook: 0x%p", LoadLevel, hLoadLevel);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)LoadLevel, hLoadLevel);
	DetourTransactionCommit();
}