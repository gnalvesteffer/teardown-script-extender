#include "LuaFunctions.h"
#include "Teardown.h"

int LuaFunctions::cLuaFunctions::lSetPlayerPos(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_objlen(L, 1) != 3)
	{
		lua_pushliteral(L, "invalid vector");
		lua_error(L);
		return 0;
	}

	lua_pushnumber(L, 1);
	lua_gettable(L, -2);
	float x = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 2);
	lua_gettable(L, -2);
	float y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 3);
	lua_gettable(L, -2);
	float z = lua_tonumber(L, -1);
	lua_pop(L, 1);

	Teardown::pGame->pPlayer->position = Vector3(x, y, z);
	return 0;
}

int LuaFunctions::cLuaFunctions::lSetPlayerVel(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_objlen(L, 1) != 3)
	{
		lua_pushliteral(L, "invalid vector");
		lua_error(L);
		return 0;
	}

	lua_pushnumber(L, 1);
	lua_gettable(L, -2);
	float x = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 2);
	lua_gettable(L, -2);
	float y = lua_tonumber(L, -1);
	lua_pop(L, 1);

	lua_pushnumber(L, 3);
	lua_gettable(L, -2);
	float z = lua_tonumber(L, -1);
	lua_pop(L, 1);

	Teardown::pGame->pPlayer->velocity = Vector3(x, y, z);
	return 0;
}