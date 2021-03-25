#pragma once
#include <string>
#include <unordered_map>

class LuaFunctionState
{
public:
    std::unordered_map<unsigned short, std::vector<std::string>> port_messages;
};

