#include "TeardownFunctions.h"

#include "Teardown.h"
#include "Entities.h"
#include "Types.h"
#include "Memory.h"
#include "Globals.h"
#include "Signatures.h"
#include "Logger.h"


typedef Vox* (*tLoadVox)		(Teardown::small_string* path, void* a2, float Scale);
tLoadVox tdLoadVox;

typedef Vox* (*tVoxFunction)	(Vox* pVox);
tVoxFunction GenVoxTexture;
tVoxFunction InitializeVox;


Vox* Teardown::Functions::EntityFunctions::LoadVox(const char* path, float Scale)
{
	Teardown::small_string voxPath(path);

	// No idea with this value does, doesn't seem to alter anything either
	char unk[16] = { 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0xB0, 0x01, 0x00, 0x00 };

	Vox* newVox = tdLoadVox(&voxPath, &unk, Scale);
	GenVoxTexture(newVox);
	InitializeVox(newVox);
	return newVox;
}

Entity* Teardown::Functions::EntityFunctions::GetEntityById(uint16_t entityId)
{
	Teardown::small_vector<Entity*>* entList = &Teardown::pGame->pScene->entityList2;

	if (!entityId || entityId > entList->size() || entityId <= 0)
		return 0;

	DWORD64 dwEntityPointer = (DWORD64)entList->begin() + (0x8 * entityId);
	return *(Entity**)dwEntityPointer;
}

void Teardown::Functions::EntityFunctions::GetAddresses()
{
	tdLoadVox = (tLoadVox)Memory::FindPattern(Signatures::EntityFunctions::LoadVox.pattern, Signatures::EntityFunctions::LoadVox.mask, Globals::HModule);

	DWORD64 dwGenVoxTexture = Memory::FindPattern(Signatures::EntityFunctions::GenVoxTexture.pattern, Signatures::EntityFunctions::GenVoxTexture.mask, Globals::HModule);
	GenVoxTexture = (tVoxFunction)Memory::readPtr(dwGenVoxTexture, 1);

	DWORD64 dwInitializeVox = Memory::FindPattern(Signatures::EntityFunctions::InitializeVox.pattern, Signatures::EntityFunctions::InitializeVox.mask, Globals::HModule);
	InitializeVox = (tVoxFunction)Memory::readPtr(dwInitializeVox, 1);

	WriteLog(LogType::Address, "LoadVox: 0x%p", tdLoadVox);
}
