#include "Hooks.h"
#include "Logger.h"
#include "Menu.h"
#include "Globals.h"
#include <detours.h>
#include "imgui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC	oWndProc;

typedef bool(*tSetCursorPos)(int X, int Y);
tSetCursorPos	oSetCursorPos;

bool hSetCursorPos(int X, int Y)
{
	if (!Menu::drawMenu)
		return oSetCursorPos(X, Y);
	return true;
}

LRESULT	APIENTRY hWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_INSERT)
		{
			Menu::drawMenu = !Menu::drawMenu;
			return true;
		}
	}

	if (Menu::drawMenu && GetForegroundWindow() == Globals::gWnd)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void Hooks::InputHooks::hookWndProc()
{
	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Globals::gWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));
}

void Hooks::InputHooks::hookCursor()
{
	HMODULE USER32 = GetModuleHandle("USER32.dll");
	oSetCursorPos = (tSetCursorPos)GetProcAddress(USER32, "SetCursorPos");

	WriteLog("SetCursorPos: 0x%p | SetCursorPos hook: 0x%p", oSetCursorPos, hSetCursorPos);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oSetCursorPos, hSetCursorPos);
	DetourTransactionCommit();
}