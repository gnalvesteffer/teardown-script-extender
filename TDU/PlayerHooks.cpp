#include "Hooks.h"
#include "Globals.h"
#include "Memory.h"
#include "Teardown.h"
#include "Logger.h"

#include <detours.h>

typedef void (*tUpdatePlayerCollisions)			(Player* Ply, void* unk1);
tUpdatePlayerCollisions UpdatePlayerCollisions;

typedef void (*tPlayerInteractionHandler)		(Player* Ply);
tPlayerInteractionHandler PlayerInteractionHandler;

typedef void(*tPlayerGrabHandler)				(Player* Ply, void* unk);
tPlayerGrabHandler PlayerGrabHandler;

typedef void	(*tUpdateCamera)				(Player* Ply, void* unk1, void* unk2, void* unk3);
tUpdateCamera UpdateCamera;

bool Hooks::PlayerHooks::updateCollisions = true;
bool Hooks::PlayerHooks::updateCamera = true;

void hUpdateCamera(Player* Ply, void* unk1, void* unk2, void* unk3)
{
    if (Hooks::PlayerHooks::updateCamera)
        return UpdateCamera(Ply, unk1, unk2, unk3);
}

void hUpdatePlayerCollisions(Player* Ply, void* unk1)
{
    if (Hooks::PlayerHooks::updateCollisions)
        return UpdatePlayerCollisions(Ply, unk1);
}

void Hooks::PlayerHooks::updateGrabCheck(bool enabled)
{
    if (enabled)
    {
        unsigned char patch[8] = { 0x41, 0x88, 0xB4, 0x24, 0xCC, 0x04, 0x00, 0x00 };  // IDA switched the 0 in (mov byte ptr [ply + 0x4C4], 0) to the sil instruction
        Memory::memcpy_s((PVOID)((DWORD64)PlayerInteractionHandler + 0xB79), (char*)patch, 8);

        unsigned char patch3[6] = { 0x0F, 0x84, 0x17, 0x02, 0x00, 0x00 };
        Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xB5), (char*)patch3, 6);

        unsigned char patch2[6] = { 0x0F, 0x86, 0x05, 0x02, 0x00, 0x00 };
        Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xC7), (char*)patch2, 6);

        unsigned char patch4[6] = { 0x0F, 0x87, 0xF0, 0x01, 0x00, 0x00 };
        Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xDC), (char*)patch4, 6);

        return;
    }

    unsigned char patch[8] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, };
    Memory::memcpy_s((PVOID)((DWORD64)PlayerInteractionHandler + 0xB79), (char*)patch, 8);

    unsigned char patch2[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
    Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xB5), (char*)patch2, 6);
    Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xC7), (char*)patch2, 6);
    Memory::memcpy_s((PVOID)((DWORD64)PlayerGrabHandler + 0xDC), (char*)patch2, 6);
}

void Hooks::PlayerHooks::initPlayerHooks()
{
    DWORD64 dwUpdateCamera = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x48\x8B\x4B\x70\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????", Globals::gModule);
    UpdateCamera = (tUpdateCamera)Memory::readPtr(dwUpdateCamera, 1);

    DWORD64 dwUpdatePlayerCollisions = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x41\x8B\xF4\x48\x8B\x07\x48\x8B\x88\x00\x00\x00\x00\x83\x39\x00\x7E\x31", "x????xxxxxxxxx????xxxxx", Globals::gModule);
    UpdatePlayerCollisions = (tUpdatePlayerCollisions)Memory::readPtr(dwUpdatePlayerCollisions, 1);

    DWORD64 dwPlyInterHandler = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x41\x0F\x28\xD4\x48\x8B\xD6\x48\x8B\xCF\xE8\x00\x00\x00\x00\x41\x0F\x28\xD4", "x????xxxxxxxxxxx????xxxx", Globals::gModule);
    PlayerInteractionHandler = (tPlayerInteractionHandler)Memory::readPtr(dwPlyInterHandler, 1);

    DWORD64 dwPlyGrabHandler = Memory::FindPattern((PBYTE)"\xE8\x00\x00\x00\x00\x41\x0F\x28\xD4\x48\x8B\xD6\x48\x8B\xCF\xE8\x00\x00\x00\x00\x41\x8B\xDE", "x????xxxxxxxxxxx????xxx", Globals::gModule);
    PlayerGrabHandler = (tPlayerGrabHandler)Memory::readPtr(dwPlyGrabHandler, 1);

    WriteLog("UpdateCamera: 0x%p | hook: 0x%p", UpdateCamera, hUpdateCamera);
    WriteLog("UpdatePlayerCollisions: 0x%p | hook: 0x%p", UpdatePlayerCollisions, hUpdatePlayerCollisions);

    WriteLog("PlayerInteractionHandler: 0x%p", PlayerInteractionHandler);
    WriteLog("PlayerGrabHandler: 0x%p", PlayerGrabHandler);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)UpdateCamera, hUpdateCamera);
    DetourTransactionCommit();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)UpdatePlayerCollisions, hUpdatePlayerCollisions);
    DetourTransactionCommit();
}