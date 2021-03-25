#include "TeardownFunctions.h"
#include "Signatures.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"

#include "Entities.h"
#include "Script.h"

typedef Entity* (*tEntityCCtor)				(void* memAlloc, Entity* Parent);
tEntityCCtor ScriptCCtor;

Script* Teardown::Functions::Constructors::newScript(void* memAlloc, Entity* Parent)
{
	return (Script*)ScriptCCtor(memAlloc, Parent);
}

void Teardown::Functions::Constructors::GetAddresses()
{
	DWORD64 dwScriptCCtor = Memory::FindPattern(Signatures::ClassConstructors::ScriptCCtor.pattern, Signatures::ClassConstructors::ScriptCCtor.mask, Globals::HModule);
	ScriptCCtor = (tEntityCCtor)Memory::readPtr(dwScriptCCtor, 1);

	WriteLog(LogType::Address, "Script::Script: 0x%p", ScriptCCtor);
}