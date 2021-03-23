#pragma once
#include <Windows.h>

namespace Hooks
{
	namespace PlayerHooks
	{
		void initPlayerHooks();
		void updateGrabCheck(bool enabled);
		extern bool updateCamera;
		extern bool updateCollisions;
	}

	namespace InputHooks
	{
		void hookWndProc();
		void hookCursor();
	}

	void initLoggerHook();
	void initRenderHooks();
	void initGameHooks();
	void initCWHook();
	void initLuaHooks();

	void initHooks();
}