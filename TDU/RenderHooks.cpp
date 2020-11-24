#include "Hooks.h"
#include "Logger.h"
#include "Globals.h"
#include "Memory.h"
#include "Menu.h"
#include "Features.h"

#include <mutex>
#include <detours.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <gl/GL.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_opengl3.h"

typedef BOOL(__stdcall * twglSwapBuffers)		(_In_ HDC hDc);
twglSwapBuffers owglSwapBuffers;

typedef void(*tglewInit)	();
tglewInit oglewInit;

ImGuiContext* ctx;

void onGLInit()
{
	WriteLog("Initializing ImGui");

	if (ctx)
	{
		ImGui::DestroyContext(ctx);
		ZeroMemory(ctx, sizeof(ctx));
		ctx = nullptr;
		WriteLog("Old context destroyed");
	}
	ctx = ImGui::CreateContext();

	const char* glsl_version = "#version 130";
	ImGui_ImplWin32_Init(Globals::gWnd);
	ImGui_ImplOpenGL3_Init(glsl_version);
	Menu::Init();
}

bool hwglSwapBuffers(_In_ HDC hDc)
{
	Features::Noclip::doNoclip();
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Menu::Draw();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return owglSwapBuffers(hDc);
}

void hGlewInit()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();

	oglewInit();
	glewInit();
	onGLInit();
}

void Hooks::initRenderHooks()
{
	HMODULE OpenGL = GetModuleHandle("C:\\Windows\\System32\\opengl32.dll");

	owglSwapBuffers = (twglSwapBuffers)GetProcAddress(OpenGL, "wglSwapBuffers");

	WriteLog("wglSwapBuffers: 0x%p | wglSwapBuffers hook: 0x%p", owglSwapBuffers, hwglSwapBuffers);

	oglewInit = (tglewInit)Memory::FindPattern((PBYTE)"\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x85\xC0\x75\x09", "xxxxx????xxxx", Globals::gModule);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)oglewInit, hGlewInit);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)owglSwapBuffers, hwglSwapBuffers);
	DetourTransactionCommit();
}