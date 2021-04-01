#pragma once
#include "Lua.hpp"

/*
	Important:
	This file should have a different name, or LuaFunctions.h/cpp should have a different name, cause it's gonna cause some confusion
*/

namespace CLuaFunctions
{
	namespace EntityFunctions
	{
		int CreateShape(lua_State* L);
		int CreateBody(lua_State* L);
		int LoadVox(lua_State* L);
		int InitializeBody(lua_State* L);
	}

	namespace UdpFunctions
	{
		int InitializeUdpPortListener(lua_State* L);
		int GetMessagesForPort(lua_State* L);
		int SendUdpMessage(lua_State* L);
	}

	namespace IOFunctions
	{
		int ReadFile(lua_State* L);
	}

	void RegisterCFunctions(lua_State* L);
}