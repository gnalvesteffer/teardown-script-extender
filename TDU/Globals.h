#pragma once
#include <string>
#include <unordered_map>
#include <Windows.h>
#include "LuaFunctionState.h"

#ifndef GLOBALS
#define GLOBALS

namespace Globals
{
    static const std::string version("0.1");
    extern HWND gWnd;
    extern HMODULE gModule;
    extern float FPS;
    extern LuaFunctionState lua_function_state;
}
#endif