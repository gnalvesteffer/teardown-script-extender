#pragma once
#include "Entities.h"
#include "Lua.hpp"

class ScriptCore_LuaStateInfo
{
public:
	lua_State *pLuaState; //0x0000
	char pad_0008[312]; //0x0008
}; // Size: 0x140

class ScriptCore_LuaState
{
public:
	char pad_0000[48]; //0x0000
	ScriptCore_LuaStateInfo *pLuaStateInfo; //0x0030

};

class ScriptCore
{
public:
	char pad_01[16]; //0x0008
	char *scriptName; //0x0010
	char pad_03[24]; //0x0018
	ScriptCore_LuaState LuaState; //0x0030
	char pad_04[384]; //0x0068

	//virtual void Destroy(ScriptCore* sc, bool freeMem);
	//virtual void RegisterGameFunctions(ScriptCore_LuaState* state);
	//virtual void Function03();
	//virtual void Function04();
	//virtual void Function05();
	//virtual void Function06();
	//virtual void Function07();
	//virtual void Function08();
	//virtual void Function09();
}; // Size: 0x1C0

class Script : public Entity
{
	char		pad_01[16];		// 0x28
	ScriptCore	sCore;			//	0x38
	char		pad_02[456];	// 0x40
}; // Size: 0x200

