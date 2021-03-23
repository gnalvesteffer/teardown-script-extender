#include "Functions.h"
#include "Memory.h"
#include "Globals.h"
#include "Types.h"
#include "Logger.h"

#include <detours.h>

typedef void* (*tLoadVox)					(small_string* path, void* a2, void* a3, void* a4, void* a5, float voxScale);
tLoadVox LoadVox;

// a3 and should be nothing, they get xor'd before getting passed to the function, 

void* hLoadVox(small_string* path, void* a2, void* a3, void* a4, void* a5, float voxScale)
{
	WriteLog("Loaded vox: %s", path->c_str());
 	return LoadVox(path, a2, a3, a4, a5, voxScale);
}

void Teardown::Functions::Entities::getFunctionAddresses()
{
	DWORD64 dwLoadVox = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\x4C\x24\x00\x84\xC0", "x????xxxx?xx", Globals::gModule);
	LoadVox = (tLoadVox)Memory::readPtr(dwLoadVox, 1);

	WriteLog("LoadVox: 0x%p | hook: 0x%p", LoadVox, hLoadVox);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)LoadVox, hLoadVox);
	DetourTransactionCommit();
}