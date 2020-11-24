#include "Menu.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/TextEditor.h"

#include "Globals.h"
#include "Features.h"
#include "Teardown.h"
#include "TLua.h"

bool Menu::drawMenu = false;

bool Menu::overrideFog = false;
float Menu::fogColour[4] = { 0.5f, 0.5f, 0.5f, 0.5f };

bool Menu::overrideAmbient = false;
float Menu::ambientColourAvg[4] = { 0.5f, 0.5f, 0.5f, 1.f };

bool Menu::overrideWater = false;
float Menu::wave = 0.f;
float Menu::ripple = 0.f;
float Menu::motion = 0.f;
float Menu::foam = 0.f;

bool Menu::overrideDOF = false;
float Menu::focusMax = 1.f;
float Menu::focusMin = 1.f;

bool Menu::overrideWetness = false;
float Menu::wetness = 1.f;
float Menu::puddleAmount = 1.f;
float Menu::puddleFreq = 1.f;

Vector3 prevPos = Vector3(0, 0, 0);
bool lockPos = false;

Vector3 prevVel = Vector3(0, 0, 0);
bool lockVel = false;

bool noAttackCooldown = false;
bool noRecoil = false;
bool godMode = false;
bool overrideFOV = false;

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
	static int newFOV = Teardown::pGame->gameSettings.FOV;

	IO->MouseDrawCursor = Menu::drawMenu;

	Globals::FPS = IO->Framerate;

	if (overrideFOV)
		if (Teardown::pGame->gameSettings.FOV != newFOV)
			Teardown::Functions::SetIntValue(&small_string("options.gfx.fov"), newFOV);

	if (!lockPos)
		prevPos = Teardown::pGame->pPlayer->position;
	else
		Teardown::pGame->pPlayer->position = prevPos;

	if (!lockVel)
		prevVel = Teardown::pGame->pPlayer->velocity;
	else
		Teardown::pGame->pPlayer->velocity = prevVel;

	if (godMode)
		Teardown::pGame->pPlayer->health = 1;

	if (noAttackCooldown)
		Teardown::pGame->pPlayer->attackCooldown = 0;

	if (noRecoil)
		Teardown::pGame->pPlayer->recoil = 0;

	if (!drawMenu)
		return;

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
			Teardown::pGame->isQuickLoading = true;
			Teardown::pGame->statusTransition = gameStatus::playing;
		}
		ImGui::SameLine();
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !Teardown::pGame->isPlaying);
		if (ImGui::Button("Write save"))
			Teardown::pGame->isQuickSaving = true;
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
			Teardown::pGame->isRestarting = true;
		ImGui::PopItemFlag();

		ImGui::Text("FPS: %f", Globals::FPS);
		ImGui::Text("Playing: %s", Teardown::pGame->isPlaying ? "true" : "false");
		ImGui::Text("Paused: %s", Teardown::pGame->isPaused ? "true" : "false");
		ImGui::Text("Current status: %s", gameStatusStr[Teardown::pGame->Status - 1]);
		ImGui::Text("Goal status: %s", Teardown::pGame->statusTransition != 0 ? gameStatusStr[Teardown::pGame->statusTransition - 1] : "None");
		if (Teardown::pGame->isPlaying)
		{
			ImGui::Text("Current level: %s", Teardown::pGame->currentLevel);
			ImGui::Text("Current level file: %s", Teardown::pGame->currentLevelFile);
		}
		else
		{
			ImGui::Text("Current level: Not playing");
			ImGui::Text("Current level file: Not playing");
		}

		if (ImGui::CollapsingHeader("Shader / render stuff"))
		{
			ImGui::Checkbox("Draw bounds", &Teardown::pGame->pShaderSystem->drawBounds);
			ImGui::Checkbox("Draw bodies", &Teardown::pGame->pShaderSystem->drawBodies);
			ImGui::Checkbox("Draw shadowvolume", &Teardown::pGame->pShaderSystem->drawShadowVolume);

			if (ImGui::BeginCombo("RenderTarget", rendertargets[currentRTIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(rendertargets); n++)
				{
					const bool is_selected = (currentRTIndex == n);
					if (ImGui::Selectable(rendertargets[n], is_selected))
					{
						Teardown::pGame->pShaderSystem->renderTarget = small_string(rendertargets[n]);
						currentRTIndex = n;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();
			ImGui::Checkbox("Override ambient", &Menu::overrideAmbient);
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(115, 0));
			ImGui::SameLine();
			ImGui::Checkbox("Override fog", &Menu::overrideFog);
			ImGui::PushItemWidth(150);
			ImGui::ColorPicker4("Ambient average", Menu::ambientColourAvg, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);
			ImGui::SameLine();
			ImGui::ColorPicker4("Fog colour", Menu::fogColour, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);
			ImGui::PopItemWidth();

			ImGui::Checkbox("Override water", &Menu::overrideWater);
			ImGui::InputFloat("uWave", &Menu::wave);
			ImGui::InputFloat("uRipple", &Menu::ripple);
			ImGui::InputFloat("uMotion", &Menu::motion);
			ImGui::InputFloat("uFoam", &Menu::foam);


			ImGui::Checkbox("Override DOF", &Menu::overrideDOF);
			ImGui::InputFloat("FocusMin", &Menu::focusMin);
			ImGui::InputFloat("FocusMax", &Menu::focusMax);

			ImGui::Checkbox("Override wetness", &Menu::overrideWetness);
			ImGui::SliderFloat("Wetness", &Menu::wetness, 0.f, 1.f);
			ImGui::SliderFloat("PuddleAmount", &Menu::puddleAmount, 0.f, 1.f);
			ImGui::SliderFloat("PuddleFreq", &Menu::puddleFreq, 0.f, 1.f);

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

		ImGui::InputFloat("Health", &Teardown::pGame->pPlayer->health);
		ImGui::SameLine();
		ImGui::Checkbox("Godmode", &godMode);

		ImGui::InputFloat("Attack cooldown", &Teardown::pGame->pPlayer->attackCooldown);
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
		ImGui::Separator();
		ImGui::SliderFloat("Noclip speed", &Features::Noclip::NoclipSpeed, 0.1f, 1.f);

		ImGui::SliderInt("FOV", &newFOV, 0, 180);
		ImGui::SameLine();
		ImGui::Checkbox("Override##FOV", &overrideFOV);

		ImGui::EndChild();
	}
	ImGui::End();

	ImGui::Begin("Script editor *unstable*", 0);

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