#include <boost/asio.hpp>
#include <unordered_map>
#include <boost/thread.hpp>
#include "LuaFunctions.h"

std::unordered_map<unsigned short, std::vector<std::string>> port_messages;
std::unordered_map<unsigned short, bool> port_message_clear_flags;

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
    const auto is_socket_open_for_port = port_messages.find(port) != port_messages.end();
    if (!is_socket_open_for_port)
    {
        auto thread = boost::thread([L, port]
        {
            boost::asio::io_service io_service;
            boost::asio::ip::udp::socket socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
            boost::asio::ip::udp::endpoint remote_endpoint;
            boost::system::error_code error;
            char receive_buffer[1024];

            while (true)
            {
                if (port_message_clear_flags[port])
                {
                    port_message_clear_flags[port] = false;
                    port_messages[port].clear();
                }

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

    static boost::asio::io_service io_service;
    static boost::asio::ip::udp::socket socket(io_service);
    if (!socket.is_open())
    {
        socket.open(boost::asio::ip::udp::v4());
    }

    const auto remote = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ip_address), port);
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

    lua_newtable(L);
    if (port_messages.find(port) != port_messages.end())
    {
        for (auto message_index = 0; message_index < port_messages[port].size(); ++message_index)
        {
            lua_pushstring(L, port_messages[port][message_index].c_str());
            lua_rawseti(L, -2, message_index + 1);
        }
        port_message_clear_flags[port] = true;
    }
    return 1;
}
