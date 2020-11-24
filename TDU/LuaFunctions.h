#pragma once
#include "Lua.hpp"

namespace LuaFunctions
{
	namespace cLuaFunctions
	{
		int lPrint(lua_State* L);
		int lIsKeyDown(lua_State* L);
		int lSetPlayerPos(lua_State* L);
		int lSetPlayerVel(lua_State* L);
	}
	void registerVKeys(lua_State* L);
	const char* luaL_tolstring(lua_State *L, int idx, size_t *len = NULL);
}