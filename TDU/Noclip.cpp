#include "Features.h"
#include "Teardown.h"
#include "Hooks.h"
#include "Globals.h"

#include <Windows.h>

bool Features::Noclip::Enabled = false;

float Features::Noclip::NoclipSpeed = 0.25f;
Vector3 newPos;
Vector3 plyPos;
Vector3 exitVel;

void Features::Noclip::toggleNoclip()
{
	Features::Noclip::Enabled = !Features::Noclip::Enabled;

	if (Enabled)
		plyPos = Teardown::pGame->pPlayer->position;
	else
	{
		Teardown::pGame->pPlayer->position = Teardown::pGame->pPlayer->cameraPosition2 - Vector3(0, 1.7, 0);
		Teardown::pGame->pPlayer->velocity = exitVel;
	}

	Hooks::setCameraEnabled(!Enabled);
}

void Features::Noclip::doNoclip()
{
	if (!Enabled)
	{
		newPos = Teardown::pGame->pPlayer->cameraPosition2;
		return;
	}

	Vector2 input = Teardown::pGame->pPlayer->moveKeys;

	Teardown::pGame->pPlayer->position = plyPos;			// Freeze the player in place
	Teardown::pGame->pPlayer->velocity = Vector3(0, 0, 0);	// Freeze velocity
	exitVel = Vector3(0, 0, 0);

	if (input.x != 0)
	{
		Vector4 cameraAngle = Teardown::pGame->pPlayer->cameraRot;
		Vector3 fwdVec;

		fwdVec.x = 2 * (cameraAngle.x * cameraAngle.z + cameraAngle.w * cameraAngle.y);
		fwdVec.y = 2 * (cameraAngle.y * cameraAngle.z - cameraAngle.w * cameraAngle.x);
		fwdVec.z = 1 - 2 * (cameraAngle.x * cameraAngle.x + cameraAngle.y * cameraAngle.y);

		fwdVec *= (input.x * NoclipSpeed);

		if (GetAsyncKeyState(VK_SHIFT))
			fwdVec *= 2.f;

		if (GetAsyncKeyState(VK_LCONTROL))
			fwdVec *= 0.25f;

		newPos -= fwdVec;
		exitVel -= fwdVec;
	}

	if (input.y != 0)
	{
		Vector4 cameraAngle = Teardown::pGame->pPlayer->cameraRot;
		Vector3 sideVec;

		sideVec.x = 1 - 2 * (cameraAngle.y * cameraAngle.y + cameraAngle.z * cameraAngle.z);
		sideVec.y = 2 * (cameraAngle.x * cameraAngle.y + cameraAngle.w * cameraAngle.z);
		sideVec.z = 2 * (cameraAngle.x * cameraAngle.z - cameraAngle.w * cameraAngle.y);

		sideVec *= (input.y * NoclipSpeed);

		if (GetAsyncKeyState(VK_SHIFT))
			sideVec *= 2.f;

		if (GetAsyncKeyState(VK_LCONTROL))
			sideVec *= 0.25f;

		newPos += sideVec;
		exitVel += sideVec;
	}

	exitVel *= Globals::FPS;

	Teardown::pGame->pPlayer->cameraPosition2 = newPos;
}