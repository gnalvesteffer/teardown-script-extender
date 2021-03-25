#include "Hooks.h"
#include "Logger.h"
#include "Signatures.h"
#include "Memory.h"
#include "Globals.h"
#include "CLuaFunctions.h"

#include "Lua.hpp"

typedef int	(*tluaL_loadbuffer)				(lua_State* L, const char* buff, size_t size, const char* name);
tluaL_loadbuffer oluaL_loadbuffer;

/*
	luaL_loadbuffer hook
	 - Can be used to register your own functions (It's called everytime a script runs, except the ones ran at TDLua.h)
	 - Can be used to intercept and modify game scripts on runtime, so that there's no need to modify the actual file
*/

int hluaL_loadbuffer(lua_State* L, const char* buff, size_t size, const char* name)
{
	CLuaFunctions::RegisterCFunctions(L);
	return oluaL_loadbuffer(L, buff, size, name);
}

void Hooks::LuaHooks::HookLoadBuffer()
{
	DWORD64 dwLoadBuffer = Memory::FindPattern(Signatures::LuaFunctions::luaL_loadbuffer.pattern, Signatures::LuaFunctions::luaL_loadbuffer.mask, Globals::HModule);
	oluaL_loadbuffer = (tluaL_loadbuffer)Memory::readPtr(dwLoadBuffer, 1);

	WriteLog(LogType::Address, "luaL_loadbuffer: 0x%p | hook: 0x%p", oluaL_loadbuffer, hluaL_loadbuffer);
}