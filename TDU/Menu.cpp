#include "Menu.h"
#include "Globals.h"
#include "Features.h"
#include "Teardown.h"
#include "TLua.h"
#include "Shaders.h"
#include "Memory.h"
#include "Hooks.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "../ImGuiColorTextEdit/TextEditor.h"

bool Menu::drawMenu = false;

Vector3 prevPos = Vector3(0, 0, 0);
bool lockPos = false;

Vector3 prevVel = Vector3(0, 0, 0);
bool lockVel = false;

bool noAttackCooldown = false;
bool noRecoil = false;
bool godMode = false;
bool grabCheck = true;

const char* rendertargets[] = { "final", "depth", "color", "normal", "smoothnormal", "rawlight", "diffuselight", "diffuse", "reflection", "fog", "composite", "bloom"};
int currentRTIndex = 0;

ImGuiIO* IO;
ImGuiStyle* Style;

TextEditor scriptEditor;

void Menu::Init()
{
	IO = &ImGui::GetIO();
	Style = &ImGui::GetStyle();

	TextEditor::LanguageDefinition LuaLang = TextEditor::LanguageDefinition::Lua();
	scriptEditor.SetLanguageDefinition(LuaLang);
}

void Menu::Draw()
{
	IO->MouseDrawCursor = Menu::drawMenu;

	Globals::FPS = IO->Framerate;

	if (!lockPos)
		prevPos = Teardown::pGame->pPlayer->Position;
	else
		Teardown::pGame->pPlayer->Position = prevPos;

	if (!lockVel)
		prevVel = Teardown::pGame->pPlayer->Velocity;
	else
		Teardown::pGame->pPlayer->Velocity = prevVel;

	if (godMode)
		Teardown::pGame->pPlayer->Health = 1;

	if (noAttackCooldown)
		Teardown::pGame->pPlayer->toolCooldown = 0;

	if (noRecoil)
		Teardown::pGame->pPlayer->recoil = 0;

	if (!drawMenu)
		return;
	
	ImGui::SetNextWindowPos(ImVec2(IO->DisplaySize.x / 20, IO->DisplaySize.y / 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(IO->DisplaySize.x / 3, IO->DisplaySize.y / 2), ImGuiCond_FirstUseEver);
	ImGui::Begin("TDU - Debug Menu", 0);
	ImVec2 CRA = ImGui::GetContentRegionAvail();
	ImGui::BeginChild("#GameInfo", ImVec2(0, CRA.y / 2), true, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Game info");
			ImGui::EndMenuBar();
		}
		ImGui::Text("Game: 0x%p", Teardown::pGame);
		ImGui::Text("Scene: 0x%p", Teardown::pGame->pScene);
		ImGui::Text("Player: 0x%p", Teardown::pGame->pPlayer);
		ImGui::Text("Renderer: 0x%p", Teardown::pGame->pShaderSystem);
		ImGui::Separator();
		if (ImGui::Button("Load save"))
		{
			Teardown::pGame->isLoadingSave = true;
			Teardown::pGame->statusTransition = Teardown::gameStatus::playing;
		}
		ImGui::SameLine();
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !Teardown::pGame->isPlaying);
		if (ImGui::Button("Write save"))
			Teardown::pGame->isSaving = true;
		ImGui::PopItemFlag();

		ImGui::Text("FPS: %f", Globals::FPS);
		ImGui::Text("Playing: %s", Teardown::pGame->isPlaying ? "true" : "false");
		ImGui::Text("Paused: %s", Teardown::pGame->isPaused ? "true" : "false");
		ImGui::Text("Current status: %s", gameStatusStr[Teardown::pGame->Status - 1]);
		ImGui::Text("Goal status: %s", Teardown::pGame->statusTransition != 0 ? gameStatusStr[Teardown::pGame->statusTransition - 1] : "None");

		ImGui::Checkbox("Draw bounds", &Teardown::pGame->pShaderSystem->drawBounds);
		ImGui::Checkbox("Draw bodies", &Teardown::pGame->pShaderSystem->drawBodies);
		ImGui::Checkbox("Draw shapes", &Teardown::pGame->pShaderSystem->drawShapes);
		ImGui::Checkbox("Draw shadowvolume", &Teardown::pGame->pShaderSystem->drawShadowVolume);

		if (ImGui::BeginCombo("Render buffer", rendertargets[currentRTIndex]))
		{
			for (int n = 0; n < IM_ARRAYSIZE(rendertargets); n++)
			{
				const bool is_selected = (currentRTIndex == n);
				if (ImGui::Selectable(rendertargets[n], is_selected))
				{
					Teardown::pGame->pShaderSystem->renderBuffer = small_string(rendertargets[n]);
					currentRTIndex = n;
				}
					
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::EndChild();
	}
	ImGui::BeginChild("#PlayerInfo", ImVec2(0, CRA.y / 2), true, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Player");
			ImGui::EndMenuBar();
		}

		ImGui::InputFloat("Health", &Teardown::pGame->pPlayer->Health);
		ImGui::SameLine();
		ImGui::Checkbox("Godmode", &godMode);

		ImGui::InputFloat("Attack cooldown", &Teardown::pGame->pPlayer->toolCooldown);
		ImGui::SameLine();
		ImGui::Checkbox("No cooldown", &noAttackCooldown);
		ImGui::SliderFloat("Recoil", &Teardown::pGame->pPlayer->recoil, 0, 1);
		ImGui::SameLine();
		ImGui::Checkbox("No recoil", &noRecoil);

		ImGui::InputFloat3("Position", (float*)&prevPos);
		ImGui::SameLine();
		ImGui::Checkbox("Lock##Pos", &lockPos);
		ImGui::InputFloat3("Velocity", (float*)&prevVel);
		ImGui::SameLine();
		ImGui::Checkbox("Lock##Vel", &lockVel);
		if (ImGui::Checkbox("Verify object before grabbing", &grabCheck))
		{
			Hooks::PlayerHooks::updateGrabCheck(grabCheck);
		}

		ImGui::Separator();
		ImGui::SliderFloat("Noclip speed", &Features::Noclip::NoclipSpeed, 1.f, 100.f);
		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2((IO->DisplaySize.x / 20 * 2) + IO->DisplaySize.x / 3, IO->DisplaySize.y / 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(IO->DisplaySize.x / 3, IO->DisplaySize.y / 2), ImGuiCond_FirstUseEver);

	ImGui::Begin("Script editor [unstable]", 0);

	ImVec2 WindowSize = ImGui::GetContentRegionAvail();
	ImVec2 textSize = ImGui::CalcTextSize("execute", NULL, true);
	ImVec2 size = ImVec2(textSize.x + Style->FramePadding.x * 2.0f, textSize.y + Style->FramePadding.y * 2.0f);
	ImVec2 childSize(WindowSize.x, WindowSize.y - size.y - Style->ItemSpacing.y);

	ImGui::BeginChild("##ScriptExecChild", childSize);
		scriptEditor.Render("scriptEditor");
	ImGui::EndChild();

	if (ImGui::Button("Run"))
		Teardown::Lua::RunScript(scriptEditor.GetText());

	ImGui::SameLine();

	if (ImGui::Button("Clear"))
		ImGui::OpenPopup("Clear?");

	if (ImGui::BeginPopupModal("Clear?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Your script will be deleted. \n(This can't be undone).");
		if (ImGui::Button("Ok"))
		{
			scriptEditor.SetText("");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}

	ImGui::End();
}