#include "Hooks.h"

#include "Script.h"
#include "Entities.h"
#include "Memory.h"
#include "Globals.h"

#include <detours.h>

typedef Script* (*tScriptCctor)		(void* mAlloc, Entity* Parent);
tScriptCctor ScriptCctor;

Script* hScriptCctor(void* mAlloc, Entity* Parent)
{
	return ScriptCctor(mAlloc, Parent);
}

void Hooks::TestHooks::hookScriptCCtor()
{
	DWORD64 dwScriptCC = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\xD8\x33\xFF", "x????xxxxx", Globals::HModule);
	ScriptCctor = (tScriptCctor)Memory::readPtr(dwScriptCC, 1);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ScriptCctor, hScriptCctor);
	DetourTransactionCommit();
}