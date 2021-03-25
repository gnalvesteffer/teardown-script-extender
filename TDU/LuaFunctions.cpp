#include "TeardownFunctions.h"
#include "Signatures.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "Script.h"

typedef void (*tRegisterGameFunctions)		(ScriptCore* pScriptCore);
tRegisterGameFunctions tdRegisterGameFunctions;

void Teardown::Functions::LuaFunctions::RegisterGameFunctions(ScriptCore* pScriptCore)
{
	tdRegisterGameFunctions(pScriptCore);
}

void Teardown::Functions::LuaFunctions::GetAddresses()
{
	tdRegisterGameFunctions = (tRegisterGameFunctions)Memory::FindPattern(Signatures::LuaFunctions::RegisterGameFunctions.pattern, Signatures::LuaFunctions::RegisterGameFunctions.mask, Globals::HModule);
	
	WriteLog(LogType::Address, "RegisterGameFunctions: 0x%p", tdRegisterGameFunctions);
}