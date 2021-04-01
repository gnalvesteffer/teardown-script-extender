#include "CLuaFunctions.h"
#include "Config.h"

void CLuaFunctions::RegisterCFunctions(lua_State* L)
{
#ifdef ENABLE_DEBUG_LIB
    luaopen_debug(L);
#endif

#ifdef ENABLE_OS_LIB
    luaopen_os(L);
#endif

#ifdef ENABLE_IO_LIB
    luaopen_io(L);
#endif

    /*
        Register your functions here
    */

    // Entities
    lua_pushcfunction(L, CLuaFunctions::EntityFunctions::CreateBody);
    lua_setglobal(L, "CreateBody");

    lua_pushcfunction(L, CLuaFunctions::EntityFunctions::CreateShape);
    lua_setglobal(L, "CreateShape");

    lua_pushcfunction(L, CLuaFunctions::EntityFunctions::LoadVox);
    lua_setglobal(L, "LoadVox");

    lua_pushcfunction(L, CLuaFunctions::EntityFunctions::InitializeBody);
    lua_setglobal(L, "InitializeBody");

    // UDP
    lua_pushcfunction(L, CLuaFunctions::UdpFunctions::InitializeUdpPortListener);
    lua_setglobal(L, "InitializeUdpPortListener");

    lua_pushcfunction(L, CLuaFunctions::UdpFunctions::GetMessagesForPort);
    lua_setglobal(L, "GetMessagesForPort");

    lua_pushcfunction(L, CLuaFunctions::UdpFunctions::SendUdpMessage);
    lua_setglobal(L, "SendUdpMessage");

    // IO
    lua_pushcfunction(L, CLuaFunctions::IOFunctions::ReadFile);
    lua_setglobal(L, "ReadFile");
}