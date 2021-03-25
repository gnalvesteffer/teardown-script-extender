#pragma once
#include "Entities.h"
#include "Lua.hpp"

class ScriptCore_LuaStateInfo
{
public:
	lua_State* pLuaState; //0x0000
	char pad_0008[80]; //0x0008
}; //Size: 0x0058

class ScriptCore_LuaState
{
public:
	char pad_0000[56]; //0x0000
	ScriptCore_LuaStateInfo* pLSInfo; //0x0038
}; //Size: 0x0040

class ScriptCore
{
public:
	char pad_0008[40]; //0x0008
	ScriptCore_LuaState SC_LuaState; //0x0030
	char pad_0070[2592]; //0x0070

	virtual void Destroy(ScriptCore* pScriptCore, bool freeMemory);
	virtual void RegisterGameFunctions(ScriptCore* pScriptCore);
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
}; //Size: 0xA90


class Script : public Entity
{
public:
	char* scriptName; //0x0028
	char pad_0030[8]; //0x0030
	class ScriptCore scriptCore; //0x0038
}; //Size: 0xAC8