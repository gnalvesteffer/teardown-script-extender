#pragma once
#include <string>
#include <Windows.h>

#ifndef GLOBALS
#define GLOBALS
namespace Globals
{
	static const std::string version("0.6");
	extern HWND gWnd;
	extern HMODULE gModule;
	extern float FPS;
}
#endif