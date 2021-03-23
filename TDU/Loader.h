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

		// enables ANSI shit
		DWORD dwMode = 0;
		GetConsoleMode(hConsole, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hConsole, dwMode);

		WriteLog("Teardown Script Extender v%s", Globals::version.c_str());
		WriteLog("--------------------------------------------------------------------------------------------------");
		WriteLog("Credits:");
		WriteLog("  Xorberax - Author of Teardown Script Extender (TDSE)");
		WriteLog("  Nahu - Author of Teardown Unleashed (TDU), the foundation of TDSE: https://github.com/nxhu64/TDU");
		WriteLog("--------------------------------------------------------------------------------------------------");

		WriteLog("Hooking CreateWindow");
		Hooks::initCWHook();
	}
}