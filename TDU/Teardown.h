#pragma once
#include "Game.h"

namespace Teardown
{
	enum gameStatus : int8_t
	{
		splashScreen = 1,
		menu = 2,
		uistart = 3,
		playing = 4,
		editor = 5,
		quitting = 6
	};

	static const char* gameStatusStr[] = {
		"Splash Screen",
		"Menu",
		"UI Startup",
		"In-Game",
		"Editing",
		"Quitting"
		};

	inline Game* pGame;
}