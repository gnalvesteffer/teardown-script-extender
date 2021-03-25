#pragma once
#include <Windows.h>
#include <Psapi.h>

namespace Memory
{
	inline SIZE_T readPtr(SIZE_T Address, UINT Offset)
	{
		if (Address == 0)
			return 0;

		if (sizeof(SIZE_T) == 8)
			return Address + (INT)((*(INT*)(Address + Offset) + Offset) + sizeof(INT));

		return (SIZE_T) * (DWORD*)(Address + Offset);
	}

	inline void writePatch(PVOID address, char* val, int bytes)
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

	inline DWORD64 FindPattern(BYTE* bMask, const char* szMask, HMODULE hModule)
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