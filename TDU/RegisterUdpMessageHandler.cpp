#include <unordered_map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "Globals.h"
#include "LuaFunctions.h"

static std::unordered_map<unsigned short, std::vector<int>> port_message_handlers;

/// <summary>
/// Usage:
/// RegisterUdpMessageHandler(port, handler_function)
///
/// Example:
/// RegisterUdpMessageHandler(1337, function(message) DebugPrint(message) end)
/// </summary>
int LuaFunctions::cLuaFunctions::lRegisterUdpMessageHandler(lua_State* L)
{
    const auto port = static_cast<unsigned short>(lua_tointeger(L, 1));
    const auto message_handler = lua_gettop(L);
    if (lua_isfunction(L, message_handler))
    {
        const auto message_handler_registry_index = luaL_ref(L, LUA_REGISTRYINDEX);
        const auto is_socket_open_for_port = port_message_handlers.find(port) != port_message_handlers.end();
        port_message_handlers[port].push_back(message_handler_registry_index);
        if (!is_socket_open_for_port)
        {
            const auto thread = boost::thread([L, port]
            {
                boost::asio::io_service io_service;
                boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
                while (true)
                {
                    char receive_buffer[2048];
                    boost::asio::ip::udp::endpoint remote_endpoint;
                    boost::system::error_code error;
                    socket.receive_from(
                        boost::asio::buffer(receive_buffer),
                        remote_endpoint,
                        0,
                        error
                    );

                    if (!error)
                    {
                        for (auto message_handler_index = 0; message_handler_index < port_message_handlers[port].size(); ++message_handler_index)
                        {
                            Globals::lua_function_state.port_messages[port].push_back(std::string(receive_buffer));
                            for (auto buffer_index = 0; buffer_index < sizeof(receive_buffer); ++buffer_index)
                            {
                                receive_buffer[buffer_index] = NULL;
                            }
                        }
                    }
                }
            });
        }
    }
    return 0;
}
