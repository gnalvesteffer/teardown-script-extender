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
	
	#if SHOW_CONSOLE
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

		WriteLog(LogType::Special, " _________  ______   __  __      ");
		WriteLog(LogType::Special, "/________/\\/_____/\\ /_/\\/_/\\     ");
		WriteLog(LogType::Special, "\\__.::.__\\/\\:::_ \\ \\\\:\\ \\:\\ \\    ");
		WriteLog(LogType::Special, "   \\::\\ \\   \\:\\ \\ \\ \\\\:\\ \\:\\ \\   ");
		WriteLog(LogType::Special, "    \\::\\ \\   \\:\\ \\ \\ \\\\:\\ \\:\\ \\  ");
		WriteLog(LogType::Special, "     \\::\\ \\   \\:\\/.:| |\\:\\_\\:\\ \\ ");
		WriteLog(LogType::Special, "      \\__\\/    \\____/_/ \\_____\\/ ");
		WriteLog(LogType::Special, "Teardown unleashed v%s", Globals::version.c_str());
		WriteLog(LogType::Special, "Shout-out to Xorberax - SK83RJOSH - Knebb\n");

		// Get the game's HMODULE to be later used (as base address) when reading memory
		Globals::HModule = GetModuleHandle(NULL);

		Hooks::BaseHooks::HookCW();
	}
}