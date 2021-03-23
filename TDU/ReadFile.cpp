#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Loader.h"
#include "LuaFunctions.h"

int LuaFunctions::cLuaFunctions::lReadFile(lua_State* L)
{
    auto file_path = lua_tostring(L, 1);

    std::ifstream file_stream;
    std::string line_text;
    std::stringstream file_text_stringstream;
    file_stream.open(file_path);
    if (file_stream.is_open())
    {
        while (getline(file_stream, line_text))
        {
            file_text_stringstream << line_text << "\n";
        }
        file_stream.close();
    }
    auto file_text = file_text_stringstream.str();

    lua_pushstring(L, file_text.c_str());
    return 1;
}
