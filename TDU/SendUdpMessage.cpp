#include <boost/asio.hpp>
#include <string>
#include "LuaFunctions.h"
#include "LuaFunctionState.h"

using namespace boost::asio;

bool send_udp_message(
    const std::string& destination_ip,
    const unsigned short port,
    const char* message
)
{
    io_service io_service;
    ip::udp::socket socket(io_service);

    auto remote = ip::udp::endpoint(ip::address::from_string(destination_ip), port);
    try
    {
        socket.open(ip::udp::v4());
        socket.send_to(buffer(message, strlen(message)), remote);
    }
    catch (const boost::system::system_error& exception)
    {
        return false;
    }

    return true;
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
    send_udp_message(ip_address, port, message);
    return 0;
}
