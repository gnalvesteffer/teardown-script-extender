// weird hack-ish way to get imgui to compile without having to add it inside the project folder, so that i can have it work as a git submodule
// this is probably not the right way to do this lol

#pragma once

#include <imgui.cpp>
#include <imgui_draw.cpp>
#include <imgui_tables.cpp>
#include <imgui_widgets.cpp>
#include <backends/imgui_impl_win32.cpp>
#include <backends/imgui_impl_opengl3.cpp>