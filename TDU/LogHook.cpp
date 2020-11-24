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
	DWORD64 dwWriteToConsole = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\x8F", "x????xxxxx", Globals::gModule);
	dwWriteToConsole = Memory::readPtr(dwWriteToConsole, 1);
	oWriteToConsole = (tWriteToConsole)dwWriteToConsole;

	WriteLog("WriteToConsole: 0x%p | WriteToConsole hook: 0x%p", oWriteToConsole, hWriteToConsole);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oWriteToConsole, hWriteToConsole);
	DetourTransactionCommit();
}