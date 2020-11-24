#include "LuaFunctions.h"
#include <iostream>

int LuaFunctions::cLuaFunctions::lPrint(lua_State* L)
{
	int nargs = lua_gettop(L);

	std::cout << "\033[103;30;1m[Lua print]\033[0m ";

	for (int i = 1; i <= nargs; ++i) {
		std::cout << luaL_tolstring(L, i) << " ";
	}
	std::cout << std::endl;

	lua_pop(L, nargs);

	return 0;
}