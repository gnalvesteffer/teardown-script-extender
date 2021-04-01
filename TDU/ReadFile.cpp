#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <boost/algorithm/string/replace.hpp>
#include "CLuaFunctions.h"
#include "Loader.h"


bool is_valid_file_path(const std::string& file_path)
{
    return file_path.rfind("CURRENT_MOD/", 0) == 0; // ensures that only files within the current mod's directory can be read
}

std::string get_current_mod_id(lua_State* L)
{
    lua_getglobal(L, "GetString");
    lua_pushstring(L, "game.mod");
    lua_pcall(L, 1, 1, 0);
    const auto *mod_id = lua_tostring(L, -1);
    lua_pop(L, 1);
    return mod_id;
}

std::string get_current_mod_path(lua_State* L)
{
    auto mod_id = get_current_mod_id(L);
    auto *mod_path_registry_key = new std::string("mods.available.");
    mod_path_registry_key->append(mod_id);
    mod_path_registry_key->append(".path");

    lua_getglobal(L, "GetString");
    lua_pushstring(L, mod_path_registry_key->c_str());
    lua_pcall(L, 1, 1, 0);
    const auto *mod_path = lua_tostring(L, -1);
    lua_pop(L, 1);

    auto* mod_path_with_trailing_slash = new std::string(mod_path);
    mod_path_with_trailing_slash->append("/");
    return mod_path_with_trailing_slash->c_str();
}

std::string hydrate_virtual_file_path(lua_State* L, const std::string &virtual_file_path)
{
    return boost::replace_all_copy(virtual_file_path, "CURRENT_MOD/", get_current_mod_path(L));
}

/// <summary>
/// Note:
/// Restricted to the current mod directory.
///
/// Usage:
/// -- reads a file named "test.txt" relative to the mod's directory (whichever mod the current level belongs to)
/// -- "CURRENT_MOD" could be used to read a file adhering to a specific file organization for a framework, etc...
/// local file_text = ReadFile("CURRENT_MOD/test.txt")
/// </summary>
int CLuaFunctions::IOFunctions::ReadFile(lua_State* L)
{
    const auto *virtual_file_path = lua_tostring(L, 1); // virtual file path would start with "MOD/", and will have the mod path hydrated later.

    if (!is_valid_file_path(virtual_file_path))
    {
        // todo: show error about invalid path
        return 0;
    }

    const auto hydrated_file_path = hydrate_virtual_file_path(L, virtual_file_path);

    std::ifstream file_stream;
    std::string line_text;
    std::stringstream file_text_stringstream;
    file_stream.open(hydrated_file_path);
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
