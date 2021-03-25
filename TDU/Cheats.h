#pragma once

namespace Cheats
{
	namespace Menu
	{
		inline bool Enabled = false;
		void Draw();
	}

	namespace Noclip
	{
		inline float Speed = 0.25f;
		inline bool Enabled = false;
		void Toggle();
		void Run(); // Probably not the best name for this function
	}
}