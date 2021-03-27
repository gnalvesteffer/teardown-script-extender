#pragma once
#include "Script.h"
#include "Entities.h"

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
			Script* newScript(Entity* Parent);
			Body* newBody(Entity* Parent);
			Shape* newShape(Entity* Parent);
		}

		namespace LuaFunctions
		{
			void GetAddresses();
			void RegisterGameFunctions(ScriptCore* pScriptCore);
		}

		namespace EntityFunctions
		{
			void GetAddresses();
			Entity* GetEntityById(uint16_t entityId);

			Vox* LoadVox(const char* path, float Scale);
		}

		void GetAddresses();
	}
}