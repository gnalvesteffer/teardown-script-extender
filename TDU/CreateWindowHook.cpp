#include "Hooks.h"
#include "Logger.h"
#include "Globals.h"
#include "Memory.h"
#include "Teardown.h"
#include "Menu.h"
#include "Functions.h"

#include <mutex>
#include <detours.h>

typedef HWND(*tCreateWindowExA)	(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

std::once_flag hasHooked;
tCreateWindowExA oCreateWindowExA;

void doInitialHooks()
{
	WriteLog("Doing initial hooks");
	Hooks::initHooks();
}

HWND hCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	WriteLog("CreateWindow called");
	std::call_once(hasHooked, doInitialHooks);
	if (!lstrcmp(lpWindowName, "Teardown"))
	{
		WriteLog("Game window created");
		char windowName[100] = "TDU ";
		strcat(windowName, Globals::version.c_str());

		Globals::gWnd = oCreateWindowExA(dwExStyle, lpClassName, windowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
		Hooks::InputHooks::hookWndProc();
		Hooks::initGameHooks();
		Teardown::Functions::getFunctionAddresses();

		return Globals::gWnd;
	}

	return oCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

void Hooks::initCWHook()
{
	HMODULE USER32 = GetModuleHandle("USER32.dll");
	oCreateWindowExA = (tCreateWindowExA)GetProcAddress(USER32, "CreateWindowExA");

	WriteLog("CreateWindowExA: 0x%p | hook: 0x%p", oCreateWindowExA, hCreateWindowExA);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oCreateWindowExA, hCreateWindowExA);
	DetourTransactionCommit();
}