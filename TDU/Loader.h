#include "Logger.h"
#include "Globals.h"
#include "Config.h"
#include "Hooks.h"
#include "Cheats.h"
#include "TDLua.h"
#include "TeardownFunctions.h"

namespace Loader
{
	inline void Init()
	{
	
	#ifdef SHOW_CONSOLE
		// allocate console for logging
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// enable escape sequences
		DWORD dwMode = 0;
		GetConsoleMode(hConsole, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hConsole, dwMode);
	#endif

		WriteLog(LogType::Special, "Teardown Script Extender v%s", Globals::version.c_str());
		WriteLog(LogType::Special, "--------------------------------------------------------------------------------------------------");
		WriteLog(LogType::Special, "Credits:");
		WriteLog(LogType::Special, "  Xorberax - Author of Teardown Script Extender (TDSE)");
		WriteLog(LogType::Special, "  Nahu - Author of Teardown Unleashed (TDU), the foundation of TDSE: https://github.com/nxhu64/TDU");
		WriteLog(LogType::Special, "--------------------------------------------------------------------------------------------------");

		// Get the game's HMODULE to be later used (as base address) when reading memory
		Globals::HModule = GetModuleHandle(NULL);

		Hooks::BaseHooks::HookCW();
	}
}