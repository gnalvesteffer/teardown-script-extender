#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <winternl.h>

namespace Memory
{
	inline bool Hook(void* pAddr, void* hAddr, int len)
	{
		if (len < 13)
			return false;

		DWORD curProtection;
		VirtualProtect(pAddr, len, PAGE_EXECUTE_READWRITE, &curProtection);

		memset(pAddr, 0x90, len);

		unsigned char patch[] = {
			0x50,														// push rax
			0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // mov rax, 00 00 00 00 00 00 00 00
			0xFF, 0xE0													// jmp rax
		};

		*(DWORD64*)&patch[3] = (DWORD64)hAddr;

		memcpy((void*)pAddr, patch, sizeof(patch));

		DWORD temp;
		VirtualProtect(pAddr, len, curProtection, &temp);
		return true;
	}

	inline SIZE_T readPtr(SIZE_T Address, UINT Offset)
	{
		if (Address == 0)
			return 0;

		if (sizeof(SIZE_T) == 8)
			return Address + (INT)((*(INT*)(Address + Offset) + Offset) + sizeof(INT));

		return (SIZE_T)*(DWORD*)(Address + Offset);

	}

	inline void memcpy_s(PVOID address, char* val, int bytes)
	{
		DWORD d, ds;
		VirtualProtect(address, bytes, PAGE_EXECUTE_READWRITE, &d);
		memcpy(address, val, bytes);
		VirtualProtect(address, bytes, d, &ds);
	}

	inline bool Compare(const BYTE* pData, const BYTE* bMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pData, ++bMask)
		{
			if (*szMask == 'x' && *pData != *bMask)
				return false;
		}
		return true;
	}

	inline DWORD64 FindPattern(BYTE *bMask, const char* szMask, HMODULE hModule)
	{
		MODULEINFO moduleInfo = { 0 };
		GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));

		DWORD64 dwBaseAddress = (DWORD64)hModule;
		DWORD64 dwModuleSize = (DWORD64)moduleInfo.SizeOfImage;

		for (DWORD64 i = 0; i < dwModuleSize; i++)
		{
			if (Compare((BYTE*)(dwBaseAddress + i), bMask, szMask))
				return (DWORD64)(dwBaseAddress + i);
		}
		return 0;
	}
}