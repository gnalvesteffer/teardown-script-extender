#pragma once
#include "Script.h"

namespace Teardown
{
	namespace Functions
	{
		namespace Mem
		{
			void* Alloc(size_t size);
			void Free(void* memBlock);
			void GetAddresses();
		}

		namespace Constructors
		{
			void GetAddresses();
			Script* newScript(void* memAlloc, Entity* Parent);
		}

		namespace LuaFunctions
		{
			void GetAddresses();
			void RegisterGameFunctions(ScriptCore* pScriptCore);
		}

		void GetAddresses();
	}
}