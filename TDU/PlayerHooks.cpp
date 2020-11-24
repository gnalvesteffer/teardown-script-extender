#include "Hooks.h"
#include "Globals.h"
#include "Memory.h"
#include "Teardown.h"
#include "Logger.h"
#include "Features.h"

#include <detours.h>

typedef void(*tUpdatePlayerTool)			(Player* Ply, float timeDelta);
tUpdatePlayerTool UpdatePlayerTool;

DWORD64 dwUpdatePlayer;

// UpdateCamera + 621    F3 0F 11 43 7C                                movss   dword ptr[rbx + 7Ch], xmm0; // Set X Camera pos
// UpdateCamera + 626    F3 0F 11 8B 84 00 00 00                       movss   dword ptr[rbx + 84h], xmm1; // Set Z camera pos
// UpdateCamera + 65F    F3 0F 11 93 80 00 00 00                       movss   dword ptr[rbx + 80h], xmm2; // Update Y camera pos

void Hooks::setCameraEnabled(bool enabled)
{
	if (!enabled)
	{
		unsigned char patch[6] = { 0x90, 0x90, 0x90, 0x90, 0x90 }; 
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x621), (char *)patch, 5);

		unsigned char patch2[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x626), (char *)patch2, 8);

		unsigned char patch3[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x65F), (char *)patch3, 8);
	}
	else
	{
		unsigned char patch[5] = { 0xF3, 0x0F, 0x11, 0x43, 0x7C };
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x621), (char *)patch, 5);

		unsigned char patch2[8] = { 0xF3, 0x0F, 0x11, 0x8B, 0x84, 0x00, 0x00, 0x00 };
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x626), (char *)patch2, 8);

		unsigned char patch3[8] = { 0xF3, 0x0F, 0x11, 0x93, 0x80, 0x00, 0x00, 0x00 };
		Memory::memcpy_s((PVOID)(dwUpdatePlayer + 0x65F), (char *)patch3, 8);
	}
}


void hUpdatePlayerTool(Player* Ply, float timeDelta)
{
	UpdatePlayerTool(Ply, timeDelta);
}

void Hooks::initPlayerHooks()
{
	dwUpdatePlayer = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\x4B\x78\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????", Globals::gModule);
	dwUpdatePlayer = Memory::readPtr(dwUpdatePlayer, 1);

	WriteLog("dwUpdatePlayer: 0x%p", dwUpdatePlayer);

	DWORD64 dwUpdatePlayerTool = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\xF3\x0F\x10\x8E\x00\x00\x00\x00\x48\x8B\x4E\x40", "x????xxxx????xxxx", Globals::gModule);
	UpdatePlayerTool = (tUpdatePlayerTool)Memory::readPtr(dwUpdatePlayerTool, 1);

	WriteLog("UpdatePlayerTool: 0x%p | UpdatePlayerTool Hook: 0x%p", UpdatePlayerTool, hUpdatePlayerTool);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)UpdatePlayerTool, hUpdatePlayerTool);
	DetourTransactionCommit();
}