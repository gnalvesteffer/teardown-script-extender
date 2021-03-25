#include "TeardownFunctions.h"
#include "Signatures.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"

typedef void (*tfree)		(void* memBlock);
tfree j_free;

typedef void* (*tmalloc_base)	(size_t size);
tmalloc_base malloc_base;

void Teardown::Functions::Mem::Free(void* memBlock)
{
	j_free(memBlock);
}

void* Teardown::Functions::Mem::Alloc(size_t size)
{
	return malloc_base(size);
}

void Teardown::Functions::Mem::GetAddresses()
{
	DWORD64 dwMalloc = Memory::FindPattern(Signatures::InternalFunctions::malloc_base.pattern, Signatures::InternalFunctions::malloc_base.mask, Globals::HModule);
	malloc_base = (tmalloc_base)Memory::readPtr(dwMalloc, 1);

	DWORD64 dwFree = Memory::FindPattern(Signatures::InternalFunctions::j_free.pattern, Signatures::InternalFunctions::j_free.mask, Globals::HModule);
	j_free = (tfree)Memory::readPtr(dwFree, 1);

	WriteLog(LogType::Address, "malloc: 0x%p", malloc_base);
	WriteLog(LogType::Address, "free: 0x%p", j_free);
}