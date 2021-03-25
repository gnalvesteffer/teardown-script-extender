#pragma once
#include "Lua.hpp"

namespace LuaFunctions
{
    namespace cLuaFunctions
    {
        int lReadFile(lua_State* L);
        int lSendUdpMessage(lua_State* L);
        int lRegisterUdpMessageHandler(lua_State* L);
        int lGetMessagesForPort(lua_State* L);
    }

    const char* luaL_tolstring(lua_State* L, int idx, size_t* len = NULL);
}
