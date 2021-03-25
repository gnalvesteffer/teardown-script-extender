#pragma once
#include "Lua.hpp"

/*
	Important:
	This file should have a different name, or LuaFunctions.h/cpp should have a different name, cause it's gonna cause some confusion
*/

namespace CLuaFunctions
{
	void RegisterCFunctions(lua_State* L);
}