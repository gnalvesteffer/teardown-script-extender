#include "Cheats.h"
#include "Globals.h"
#include "Teardown.h"
#include "Types.h"
#include "TDLua.h"

#include <imgui.h>
#include <TextEditor.h>
#include <mutex>

ImGuiIO* IO;
ImGuiStyle* Style;

TextEditor scriptEditor;

std::once_flag hasInitializedMenu;

void Init()
{
	IO = &ImGui::GetIO();
	Style = &ImGui::GetStyle();

	TextEditor::LanguageDefinition LuaLang = TextEditor::LanguageDefinition::Lua();
	scriptEditor.SetLanguageDefinition(LuaLang);
}

const char* rendertargets[] = { "final", "depth", "color", "normal", "smoothnormal", "rawlight", "diffuselight", "diffuse", "reflection", "fog", "composite", "bloom" };
int currentRTIndex = 0;

bool God = false;
bool noCooldown = false;
bool noRecoil = false;

void Cheats::Menu::Draw()
{
	std::call_once(hasInitializedMenu, Init);

	Globals::FPS = IO->Framerate;

	if (God)
		Teardown::pGame->pPlayer->Health = 1;

	if (noCooldown)
		Teardown::pGame->pPlayer->toolCooldown = 0;

	if (noRecoil)
		Teardown::pGame->pPlayer->recoil = 0;

	IO->MouseDrawCursor = Enabled;
	if (!Enabled)
		return;

	ImGui::SetNextWindowPos(ImVec2(IO->DisplaySize.x / 20, IO->DisplaySize.y / 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(IO->DisplaySize.x / 3, IO->DisplaySize.y / 2), ImGuiCond_FirstUseEver);

	ImGui::Begin("TDU - Debug Menu", 0);

	ImVec2 CRA = ImGui::GetContentRegionAvail();
	ImGui::BeginChild("#GameInfo", ImVec2(0, CRA.y / 2), true, ImGuiWindowFlags_MenuBar); {
		if (ImGui::BeginMenuBar()) {
			ImGui::Text("Game info");
			ImGui::EndMenuBar();
		}

		ImGui::Text("FPS: %f", Globals::FPS);
		ImGui::Text("Playing: %s", Teardown::pGame->isPlaying ? "true" : "false");
		ImGui::Text("Paused: %s", Teardown::pGame->isPaused ? "true" : "false");
		ImGui::Text("Current state: %s", Teardown::gameStateStr[Teardown::pGame->State - 1]);

		ImGui::Separator();

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
					Teardown::pGame->pShaderSystem->renderBuffer = Teardown::small_string(rendertargets[n]);
					currentRTIndex = n;
				}

				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
	ImGui::EndChild();

	ImGui::BeginChild("#PlayerInfo", ImVec2(0, CRA.y / 2), true, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar())
		{
			ImGui::Text("Player");
			ImGui::EndMenuBar();
		}

		ImGui::InputFloat("Health", &Teardown::pGame->pPlayer->Health);
		ImGui::SameLine();
		ImGui::Checkbox("Godmode", &God);

		ImGui::InputFloat("Attack cooldown", &Teardown::pGame->pPlayer->toolCooldown);
		ImGui::SameLine();
		ImGui::Checkbox("No cooldown", &noCooldown);

		ImGui::SliderFloat("Recoil", &Teardown::pGame->pPlayer->recoil, 0, 1);
		ImGui::SameLine();
		ImGui::Checkbox("No recoil", &noRecoil);

		ImGui::InputFloat3("Position", (float*)&Teardown::pGame->pPlayer->Position);
		ImGui::InputFloat3("Velocity", (float*)&Teardown::pGame->pPlayer->Velocity);

		ImGui::Separator();
		ImGui::SliderFloat("Noclip speed", &Cheats::Noclip::Speed, 0.f, 1.f);
		ImGui::EndChild();
	}

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2((IO->DisplaySize.x / 20 * 2) + IO->DisplaySize.x / 3, IO->DisplaySize.y / 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(IO->DisplaySize.x / 3, IO->DisplaySize.y / 2), ImGuiCond_FirstUseEver);

	ImGui::Begin("Script editor", 0);

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