#pragma once
#include <Windows.h>

namespace Hooks
{
	void initHooks();
	void initLoggerHook();
	void initRenderHooks();
	void initInputHook();
	void initCursorHook();
	void initMLHook();
	void initCWHook();
	void initLuaHooks();
	void initPlayerHooks();

	void setCameraEnabled(bool enabled);
}