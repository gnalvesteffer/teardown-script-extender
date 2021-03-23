// weird way to implement ImGui as a submodule, but it works lol

#include <imgui.cpp>
#include <imgui_widgets.cpp>
#include <imgui_tables.cpp>
#include <imgui_draw.cpp>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include <backends/imgui_impl_opengl3.cpp>
#include <backends/imgui_impl_win32.cpp>