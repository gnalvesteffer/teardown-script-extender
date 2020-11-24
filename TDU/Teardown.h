#pragma once
#include "Game.h"

namespace Teardown
{
	extern Game* pGame;
	void GetFunctionAddresses();
	namespace Functions
	{
		namespace Data
		{
			void GetFunctionAddresses();
		}

		namespace Shaders
		{
			void GetFunctionAddresses();
		}
		void SetIntValue(small_string *name, int value);
	}
}