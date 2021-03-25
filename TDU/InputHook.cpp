#include "Hooks.h"
#include "Logger.h"
#include "Globals.h"
#include "Cheats.h"

#include <detours.h>
#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC	WndProc;

typedef bool(*tSetCursorPos)(int X, int Y);
tSetCursorPos	oSetCursorPos;

bool hSetCursorPos(int X, int Y)
{
	if (!Cheats::Menu::Enabled)
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
			Cheats::Menu::Enabled = !Cheats::Menu::Enabled;
			return true;
		}

		if (wParam == 0x4E && !Cheats::Menu::Enabled)
		{
			Cheats::Noclip::Toggle();
			return true;
		}
	}

	if (Cheats::Menu::Enabled && GetForegroundWindow() == Globals::HWnd)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(WndProc, hWnd, uMsg, wParam, lParam);
}

void Hooks::InputHooks::HookWndProc()
{
	WndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(Globals::HWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hWndProc)));
}

void Hooks::InputHooks::HookCursorPos()
{
	HMODULE hUSER32 = GetModuleHandle("USER32.dll");
	oSetCursorPos = (tSetCursorPos)GetProcAddress(hUSER32, "SetCursorPos");

	WriteLog(LogType::Address, "SetCursorPos: 0x%p | hook: 0x%p", SetCursorPos, hSetCursorPos);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oSetCursorPos, hSetCursorPos);
	DetourTransactionCommit();
}