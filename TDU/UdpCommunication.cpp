#include <boost/asio.hpp>
#include <unordered_map>
#include <boost/thread.hpp>
#include "LuaFunctions.h"

std::unordered_map<unsigned short, std::vector<std::string>> port_messages;
std::unordered_map<unsigned short, std::vector<int>> port_message_handlers;

/// <summary>
/// Usage:
/// InitializeUdpPortListener(port)
///
/// Example:
/// InitializeUdpPortListener(1337)
/// </summary>
int LuaFunctions::cLuaFunctions::lInitializeUdpPortListener(lua_State* L)
{
    const auto port = static_cast<unsigned short>(lua_tointeger(L, 1));
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
                const auto receive_size = socket.receive_from(
                    boost::asio::buffer(receive_buffer),
                    remote_endpoint,
                    0,
                    error
                );

                if (error || receive_size == 0)
                {
                    continue;
                }

                port_messages[port].push_back(std::string(receive_buffer, receive_size));
                for (auto buffer_index = 0; buffer_index < sizeof(receive_buffer); ++buffer_index)
                {
                    receive_buffer[buffer_index] = NULL;
                }
            }
        });
    }
    return 0;
}

/// <summary>
/// Usage:
/// SendUdpMessage(ip_address, port, message)
///
/// Example:
/// SendUdpMessage("127.0.0.1", 1337, "Hello World!")
/// </summary>
int LuaFunctions::cLuaFunctions::lSendUdpMessage(lua_State* L)
{
    const auto* ip_address = lua_tostring(L, 1);
    const auto port = static_cast<unsigned short>(lua_tointeger(L, 2));
    const auto* message = lua_tostring(L, 3);

    boost::asio::io_service io_service;
    boost::asio::ip::udp::socket socket(io_service);
    auto remote = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip_address), port);
    socket.open(boost::asio::ip::udp::v4());
    socket.send_to(boost::asio::buffer(message, strlen(message)), remote);

    return 0;
}

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

    if (port_messages.find(port) == port_messages.end())
    {
        lua_newtable(L);
        return 1;
    }

    lua_newtable(L);
    for (auto message_index = 0; message_index < port_messages[port].size(); ++message_index)
    {
        lua_pushstring(L, port_messages[port][message_index].c_str());
        lua_rawseti(L, -2, message_index + 1);
    }
    port_messages[port].clear();
    return 1;
}
