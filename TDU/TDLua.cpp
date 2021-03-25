#include "TDLua.h"
#include "Signatures.h"
#include "Entities.h"
#include "Script.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "TeardownFunctions.h"

#include "Lua.hpp"

#include <detours.h>

const char* luaL_tolstring(lua_State* L, int idx, size_t* len)
{
	if (!luaL_callmeta(L, idx, "__tostring")) {
		switch (lua_type(L, idx)) {
		case LUA_TNUMBER:
		case LUA_TSTRING:
			lua_pushvalue(L, idx);
			break;
		case LUA_TBOOLEAN: {
			int val = lua_toboolean(L, idx);
			lua_pushstring(L, val ? "true" : "false");
			break;
		}
		case LUA_TNIL:
			lua_pushliteral(L, "nil");
			break;
		default:
			lua_pushfstring(L, "%s: %p", luaL_typename(L, idx),
				lua_topointer(L, idx));
		}
	}

	return lua_tolstring(L, -1, len);
}

void Teardown::Lua::RunScript(std::string script)
{
	void* scriptAlloc = Teardown::Functions::Mem::Alloc(sizeof(Script));
	Teardown::Functions::Constructors::newScript(scriptAlloc, 0);

	Script* newScript = (Script*)scriptAlloc;

	lua_State* L = newScript->scriptCore.SC_LuaState.pLSInfo->pLuaState;
	luaopen_debug(L);

	Teardown::Functions::LuaFunctions::RegisterGameFunctions(&newScript->scriptCore);

	/*
		You can register your functions here, using lua_pushcfunction
		Or do whatever you'd like with the state
	*/

	if (luaL_loadbuffer(L, script.c_str(), script.length(), "TDU Lua") || lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		const char* lastError = luaL_tolstring(L, -1, NULL);
		WriteLog(LogType::Error, "Script error: %s", lastError);
		lua_pop(L, 2);
	}

	lua_pop(L, 1);
}