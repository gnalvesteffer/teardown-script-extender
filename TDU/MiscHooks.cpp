#include "Hooks.h"
#include "Logger.h"
#include "Globals.h"
#include "Signatures.h"
#include "Types.h"
#include "Memory.h"
#include "Config.h"

#include <detours.h>

void hLog(Teardown::small_string* logMessage)
{
	#ifndef PRINT_ERRORS_ONLY
		std::cout << "\033[97;42;1m[Game log]\033[0m " << logMessage->c_str();
	#endif
}

void Hooks::MiscHooks::HookLogFunction()
{
	DWORD64 Log = Memory::FindPattern(Signatures::Log.pattern, Signatures::Log.mask, Globals::HModule);
	WriteLog(LogType::Address, "Log: 0x%p | hook: 0x%p", Log, hLog);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Log, hLog);
	DetourTransactionCommit();
}