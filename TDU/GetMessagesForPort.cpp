#include <boost/asio.hpp>

#include "Globals.h"
#include "LuaFunctions.h"
#include "LuaFunctionState.h"


/// <summary>
/// Usage:
/// GetMessagesForPort(port)
///
/// Example:
/// local messages = GetMessagesForPort(1337) -- table of messages (strings)
/// </summary>
int LuaFunctions::cLuaFunctions::lGetMessagesForPort(lua_State* L)
{
    const auto port = static_cast<unsigned short>(lua_tointeger(L, 1));

    if (Globals::lua_function_state.port_messages.find(port) == Globals::lua_function_state.port_messages.end())
    {
        return 0;
    }

    lua_newtable(L);
    for (auto message_index = 0; message_index < Globals::lua_function_state.port_messages[port].size(); ++message_index)
    {
        lua_pushstring(L, Globals::lua_function_state.port_messages[port][message_index].c_str());
        lua_rawseti(L, -2, message_index + 1);
    }
    Globals::lua_function_state.port_messages[port].clear();
    return 1;
}
