#include "Logger.h"
#include "Hooks.h"
#include "Globals.h"

namespace Loader
{
	inline void Init()
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


		DWORD dwMode = 0;
		GetConsoleMode(hConsole, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hConsole, dwMode);

		WriteLog(" _________  ______   __  __      ");
		WriteLog("/________/\\/_____/\\ /_/\\/_/\\     ");
		WriteLog("\\__.::.__\\/\\:::_ \\ \\\\:\\ \\:\\ \\    ");
		WriteLog("   \\::\\ \\   \\:\\ \\ \\ \\\\:\\ \\:\\ \\   ");
		WriteLog("    \\::\\ \\   \\:\\ \\ \\ \\\\:\\ \\:\\ \\  ");
		WriteLog("     \\::\\ \\   \\:\\/.:| |\\:\\_\\:\\ \\ ");
		WriteLog("      \\__\\/    \\____/_/ \\_____\\/ ");
		WriteLog("Teardown unleashed v%s", Globals::version.c_str());

		WriteLog("Initializing hooks");
		Hooks::initCWHook();
	}
}