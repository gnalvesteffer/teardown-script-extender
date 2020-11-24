#include "Hooks.h"
#include "Logger.h"
#include "Menu.h"
#include "Globals.h"
#include "Features.h"

#include <detours.h>

#include "ImGui/imgui_impl_win32.h"

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

		if (wParam == 0x4E && !Menu::drawMenu)
		{
			Features::Noclip::toggleNoclip();
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

void Hooks::initInputHook()
{
	oWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Globals::gWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));
}

void Hooks::initCursorHook()
{
	HMODULE USER32 = GetModuleHandle("USER32.dll");
	oSetCursorPos = (tSetCursorPos)GetProcAddress(USER32, "SetCursorPos");

	WriteLog("SetCursorPos: 0x%p | SetCursorPos hook: 0x%p", oSetCursorPos, hSetCursorPos);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oSetCursorPos, hSetCursorPos);
	DetourTransactionCommit();
}