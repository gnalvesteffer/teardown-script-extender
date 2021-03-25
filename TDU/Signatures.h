#pragma once
#include <Windows.h>

class Signature
{
public:
	Signature(const char* nPattern, const char* nMask)
	{
		pattern = (PBYTE)nPattern; mask = nMask;
	}
	PBYTE pattern;
	const char* mask;
};

/*
	All signatures are stored here, if you implement any new functions / hooks / signatures, i suggest you keep the signatures here, so that it's easier to update whenever Teardown updates
	"direct reference" means the sig does not point to the function itself, but to a pointer of the function (a place where it's being called)
	In order to get that address, you'll have to call Memory::readPtr(sigAddress, 1), or just dereference it, whichever you prefer
*/

namespace Signatures
{

	// Game's Main function, which is called before the SwapBuffers call inside Teardown's init function, which is called on WinMain 
	inline Signature Main("\xE8\x00\x00\x00\x00\x48\x8D\x4D\xA7\xE8\x00\x00\x00\x00\xEB\x1D", "x????xxxxx????xx"); // direct reference
		
	// Log function, can be found by xref-ing "Starting up in resolution:" then getting the function 5 calls below
	inline Signature Log("\x80\x79\x0F\x00\x74\x03\x48\x8B\x09\x48\x8B\xD1\x48\x8D\x0D\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????");

	// Right below the wglMakeCurrent call on the function that creates the window, xref "Failed to call wglMakeCurrent for OpenGL context 3.3" for context
	inline Signature glewInit("\x48\x83\xEC\x28\xE8\x00\x00\x00\x00\x85\xC0\x75\x09", "xxxxx????xxxx");

	namespace LuaFunctions {
		// You can find it by searching any functions that are not from the UI lib (i.e. SetTag, or IsBodyBroken)
		inline Signature RegisterGameFunctions("\x40\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x85\x00\x00\x00\x00\x00\x00\x00\x00", "xxxxxx????xxx????xxx????????");

		// There's a bunch of ways to find this function, but there's no straight forward way, if you need to find this function dm me
		inline Signature luaL_loadbuffer("\xE8\x00\x00\x00\x00\x85\xC0\x75\x07\xB8\x00\x00\x00\x00\xEB\x57", "x????xxxxx????xx"); // direct reference
	}

	namespace PlayerFunctions {
		// Found by seeing what writes to the camera's position pointer on CE
		inline Signature UpdateCamera("\xE8\x00\x00\x00\x00\x48\x8B\x4B\x70\xF3\x0F\x10\x8B\x00\x00\x00\x00", "x????xxxxxxxx????"); // direct reference

		// Found by seeing what writes to the Player->Position.Y position
		inline Signature UpdatePlayerCollisions("\xE8\x00\x00\x00\x00\x41\x8B\xF4\x48\x8B\x07\x48\x8B\x88\x00\x00\x00\x00\x83\x39\x00\x7E\x31", "x????xxxxxxxxx????xxxxx"); // direct reference

		// Found by seeing what writes to Player->pInteractableShape
		inline Signature InteractionHandler("\xE8\x00\x00\x00\x00\x41\x0F\x28\xD4\x48\x8B\xD6\x48\x8B\xCF\xE8\x00\x00\x00\x00\x41\x0F\x28\xD4", "x????xxxxxxxxxxx????xxxx"); // direct reference
	}

	// There's a couple ways to find these, easiest way to find them is to look in the function that parses the level's xml and spawns every entity in it
	namespace ClassConstructors {
		inline Signature ScriptCCtor("\xE8\x00\x00\x00\x00\x48\x8B\xD8\x33\xFF", "x????xxxxx"); // direct reference
	}

	// Really important functions
	namespace InternalFunctions {
		// Malloc can be found xref-ing any of the class constructors
		inline Signature malloc_base("\xE8\x00\x00\x00\x00\x49\x89\x47\x10", "x????xxxx"); // direct reference

		// Can be found called on the destroy functions of any Entity vtable (1st function on Entity, ScriptCore, etc)
		inline Signature j_free("\xE8\x00\x00\x00\x00\x4C\x8B\xF5", "x????xxx"); // direct reference
	}
}