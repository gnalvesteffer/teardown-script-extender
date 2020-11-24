#include <Windows.h>
#include <detours.h>
#include <algorithm>

#include "Teardown.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"

#include "Menu.h"

#include <GL/glew.h>
#include <gl/GL.h>

typedef void*	(*tSetupShader)						(void* pData, void* a2, small_string* shader, small_string* includes, small_string* params);
tSetupShader SetupShader;

typedef void	(*tSetShaderSampler2D)				(GLuint* program, const GLchar *name, GLint* value);
tSetShaderSampler2D SetShaderSampler2D;

typedef void	(*tSetShaderVec)					(GLuint *program, GLint location, const GLfloat *value);
tSetShaderVec SetShaderVec4;
tSetShaderVec SetShaderVec3;
tSetShaderVec SetShaderVec2;

typedef void	(*tSetShaderFloat)					(GLuint *program, GLint location, float value);
tSetShaderFloat SetShaderFloat;

typedef GLint	(*tglGetUniformLocation)			(GLuint *program, const GLchar *name);
tglGetUniformLocation tdglGetUniformLocation;

void hSetShaderVec4(GLuint *program, GLint location, const GLfloat *value)
{
	if (location != -1)
	{
		if (Menu::overrideAmbient && tdglGetUniformLocation(program, "uAmbientColorAvg") == location)
		{
			const GLfloat* newAmbient = (GLfloat*)&Menu::ambientColourAvg;
			return SetShaderVec4(program, location, newAmbient);
		}

		if (Menu::overrideFog && tdglGetUniformLocation(program, "uFogColor") == location)
		{
			const GLfloat* newFogColour = (GLfloat*)&Menu::fogColour;
			SetShaderVec4(program, location, newFogColour);
			return;
		}
	}
	SetShaderVec4(program, location, value);
}

void hSetShaderFloat(GLuint *program, GLint location, float value)
{
	if (location != -1)
	{
		if (Menu::overrideDOF)
		{
			if (location == tdglGetUniformLocation(program, "uFocusMin"))
				return SetShaderFloat(program, location, Menu::focusMin);

			if (location == tdglGetUniformLocation(program, "uFocusMax"))
				return SetShaderFloat(program, location, Menu::focusMax);
		}

		if (Menu::overrideWetness)
		{
			if (location == tdglGetUniformLocation(program, "uWetness"))
				return SetShaderFloat(program, location, Menu::wetness);

			if (location == tdglGetUniformLocation(program, "uPuddleAmount"))
				return SetShaderFloat(program, location, Menu::puddleAmount);

			if (location == tdglGetUniformLocation(program, "uPuddleFreq"))
				return SetShaderFloat(program, location, Menu::puddleFreq);
		}

		if (Menu::overrideWater)
		{
			if (location == tdglGetUniformLocation(program, "uFoam"))
			{
				return SetShaderFloat(program, location, Menu::foam);
			}

			if (location == tdglGetUniformLocation(program, "uWave"))
			{
				return SetShaderFloat(program, location, Menu::wave);
			}

			if (location == tdglGetUniformLocation(program, "uMotion"))
			{
				return SetShaderFloat(program, location, Menu::motion);
			}

			if (location == tdglGetUniformLocation(program, "uRipple"))
			{
				return SetShaderFloat(program, location, Menu::ripple);
			}
		}
	}
	SetShaderFloat(program, location, value);
}

void Teardown::Functions::Shaders::GetFunctionAddresses()
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

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)SetShaderVec4, hSetShaderVec4);
	DetourTransactionCommit();

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)SetShaderFloat, hSetShaderFloat);
	DetourTransactionCommit();
}