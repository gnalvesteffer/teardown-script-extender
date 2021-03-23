#include "Hooks.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"

#include <detours.h>

typedef void(*tWriteToConsole)		(char **a1);

tWriteToConsole oWriteToConsole;

void hWriteToConsole(char **a1)
{
	std::cout << "\033[97;42;1m[Game log]\033[0m " << *a1;
}

void Hooks::initLoggerHook()
{
	DWORD64 dwWriteToConsole = Memory::FindPattern((PBYTE)"\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????", Globals::gModule);
	oWriteToConsole = (tWriteToConsole)dwWriteToConsole;

	WriteLog("WriteToConsole: 0x%p | WriteToConsole hook: 0x%p", oWriteToConsole, hWriteToConsole);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oWriteToConsole, hWriteToConsole);
	DetourTransactionCommit();
}