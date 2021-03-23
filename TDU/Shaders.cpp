#include <Windows.h>
#include <detours.h>

#include "Teardown.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "Functions.h"

#include "Menu.h"

#include <detours.h>

tglGetUniformLocation Teardown::Functions::Shaders::tdglGetUniformLocation;
tSetShaderFloat Teardown::Functions::Shaders::SetShaderFloat;
tSetShaderVec Teardown::Functions::Shaders::SetShaderVec4;
tSetShaderVec Teardown::Functions::Shaders::SetShaderVec3;
tSetShaderVec Teardown::Functions::Shaders::SetShaderVec2;
tSetupShader Teardown::Functions::Shaders::SetupShader;
tSetShaderSampler2D Teardown::Functions::Shaders::SetShaderSampler2D;

void Teardown::Functions::Shaders::getFunctionAddresses()
{
	DWORD64 dwSetShaderValue = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xC0\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xB0", "x????xxxxxx????xxxxx", Globals::gModule);
	SetupShader = (tSetupShader)Memory::readPtr(dwSetShaderValue, 1);

	DWORD64 dwSetShaderSampler2D = Memory::FindPattern((PBYTE)"\x45\x8B\x40\x0C\xE9\x00\x00\x00\x00", "xxxxx????", Globals::gModule);
	SetShaderSampler2D = (tSetShaderSampler2D)dwSetShaderSampler2D;

	DWORD64 dwSetShaderVec4 = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\xC1\xE7\x06", "x????xxxx", Globals::gModule);
	SetShaderVec4 = (tSetShaderVec)Memory::readPtr(dwSetShaderVec4, 1);

	DWORD64 dwSetShaderVec3 = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x8B\xFE\x41\x38\xB6\x00\x00\x00\x00", "x????xxxxx????", Globals::gModule);
	SetShaderVec3 = (tSetShaderVec)Memory::readPtr(dwSetShaderVec3, 1);

	DWORD64 dwSetShaderVec2 = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x85\xF6\x74\x42", "x????xxxxx", Globals::gModule);
	SetShaderVec2 = (tSetShaderVec)Memory::readPtr(dwSetShaderVec2, 1);

	DWORD64 dwSetShaderFloat = Memory::FindPattern((PBYTE)"\x83\xFA\xFF\x74\x4B", "xxxxx", Globals::gModule);
	SetShaderFloat = (tSetShaderFloat)dwSetShaderFloat;

	DWORD64 dwglGetUniformLocation = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x89\x45\xB8", "x????xxx", Globals::gModule);
	tdglGetUniformLocation = (tglGetUniformLocation)Memory::readPtr(dwglGetUniformLocation, 1);

	WriteLog("SetShaderSampler2D: 0x%p", SetShaderSampler2D);
	WriteLog("SetupShader: 0x%p", SetupShader);
	WriteLog("SetShaderVec4: 0x%p", SetShaderVec4);
	WriteLog("SetShaderVec3: 0x%p", SetShaderVec3);
	WriteLog("SetShaderVec2: 0x%p", SetShaderVec2);
	WriteLog("SetShaderFloat: 0x%p", SetShaderFloat);
	WriteLog("tdglGetUniformLocation: 0x%p", tdglGetUniformLocation);
}