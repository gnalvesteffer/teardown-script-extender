#include "TLua.h"
#include "Hooks.h"
#include "Lua.hpp"
#include "Memory.h"
#include "Logger.h"
#include "Globals.h"
#include "Script.h"
#include "Types.h"
#include "LuaFunctions.h"

#include <detours.h>
#include <vector>

typedef int	(*tluaL_loadbuffer)				(lua_State*L, const char *buff, size_t size, const char* name);
tluaL_loadbuffer oluaL_loadbuffer;

typedef int(*tlua_gettop)					(lua_State *L);
tlua_gettop olua_gettop;

lua_State *TLuaState;

std::vector<std::string> scriptQueue;


int hluaL_loadbuffer(lua_State *L, const char *buff, size_t size, const char *name)
{
	WriteLog("loaded script: %s (size: %i)", name, size);
	return oluaL_loadbuffer(L, buff, size, name);
}

// Currently unused, it used to work (no idea now) but it's unfinished
//lua_State* Teardown::Lua::CreateLuaState()
//{
//	ScriptCore* fakeSC = new ScriptCore;
//	ScriptCore_LuaState fakeSCLS;
//	fakeSCLS.pLuaStateInfo = new ScriptCore_LuaStateInfo;
//	fakeSC->LuaState = fakeSCLS;
//
//	lua_State* L = lua_open();
//
//	luaopen_base(L);
//	luaopen_debug(L);
//	luaopen_string(L);
//	luaopen_math(L);
//
//	fakeSC->LuaState.pLuaStateInfo->pLuaState = L;
//
//	RegisterGameFunctions((void*)fakeSC);
//
//	lua_getglobal(L, "_G");
//	lua_pushcfunction(L, LuaFunctions::cLuaFunctions::lPrint);
//	lua_setfield(L, -2, "print");
//	lua_pop(L, 1);
//
//	return L;
//}

void Teardown::Lua::RunScript(std::string script)
{
	if (script.empty())
		return;

	scriptQueue.push_back(script);
}

// threadsafe placeholder execution method
// there's another way but i haven't reversed enough to get the tick, update and init functions to work, yet
// runs on any random script though, so it might end up registering the same function over and over on the same state, not having UI function access on some cases, and have other inconsistent stuff
void doExecution(lua_State* L)
{
	int top = lua_gettop(L);
	std::string currentScript = scriptQueue.back();
	scriptQueue.pop_back();

	lua_getglobal(L, "_G");
	lua_pushcfunction(L, LuaFunctions::cLuaFunctions::lPrint);
	lua_setfield(L, -2, "print");
	lua_pushcfunction(L, LuaFunctions::cLuaFunctions::lSetPlayerVel);
	lua_setfield(L, -2, "SetPlayerVel");
	lua_pop(L, 1);

	luaopen_debug(L);

	if (luaL_loadbuffer(L, currentScript.c_str(), currentScript.length(), "TDU Lua") || lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		const char* lastError = LuaFunctions::luaL_tolstring(L, -1, NULL);
		WriteError("Script error: %s", lastError);
		lua_pop(L, 2);
	}

	lua_pop(L, 1);
	int top2 = lua_gettop(L);
}

int hlua_gettop(lua_State* L)
{
	if (!scriptQueue.empty())
		doExecution(L);

	return lua_gettop(L);
}

void Hooks::initLuaHooks()
{
	DWORD64 dwLoadBuffer = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x85\xC0\x75\x07\xB8\x00\x00\x00\x00\xEB\x57", "x????xxxxx????xx", Globals::gModule);
	oluaL_loadbuffer = (tluaL_loadbuffer)Memory::readPtr(dwLoadBuffer, 1);

	olua_gettop = (tlua_gettop)Memory::FindPattern((PBYTE)"\x48\x8B\x41\x10\x48\x2B\x41\x18", "xxxxxxxx", Globals::gModule);

	WriteLog("luaL_loadbuffer: 0x%p | hook: 0x%p", oluaL_loadbuffer, hluaL_loadbuffer);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oluaL_loadbuffer, hluaL_loadbuffer);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)olua_gettop, hlua_gettop);
	DetourTransactionCommit();
}