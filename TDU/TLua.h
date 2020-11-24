#pragma once
#include "Lua.hpp"
#include <string>

namespace Teardown
{
	namespace Lua
	{
		lua_State* CreateLuaState();
		void RunScript(std::string script);
	}
}