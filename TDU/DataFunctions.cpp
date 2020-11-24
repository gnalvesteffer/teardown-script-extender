#include <Windows.h>

#include "Teardown.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"

// To-do:
// SetFloatValue, SetBoolValue, etc

typedef void (*tSetIntValue)				(void* pData, small_string* name, int value);
tSetIntValue tdSetIntValue;

void Teardown::Functions::SetIntValue(small_string* name, int value)
{
	tdSetIntValue(Teardown::pGame->pValues, name, value);
}


void Teardown::Functions::Data::GetFunctionAddresses()
{
	DWORD64 dwSetIntValue = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x90\x48\x8D\x4D\xB7\xE8\x00\x00\x00\x00\x41\xBE\x00\x00\x00\x00", "x????xxxxxx????xx????", Globals::gModule);
	tdSetIntValue = (tSetIntValue)Memory::readPtr(dwSetIntValue, 1);

	WriteLog("SetIntValue: 0x%p", tdSetIntValue);
}