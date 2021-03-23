#include "Features.h"
#include "Teardown.h"
#include "Hooks.h"
#include "Globals.h"
#include "Logger.h"

#include <Windows.h>

#include <glm/gtc/quaternion.hpp>

bool Features::Noclip::Enabled = false;

float Features::Noclip::NoclipSpeed = 15.f;
Vector3 newPos;
Vector3 exitVel;

Vector3 heightOffset(0, 1.7, 0);

glm::quat newRot;

void Features::Noclip::toggleNoclip()
{
	Features::Noclip::Enabled = !Features::Noclip::Enabled;

	Hooks::PlayerHooks::updateCamera = !Features::Noclip::Enabled;
	Hooks::PlayerHooks::updateCollisions = !Features::Noclip::Enabled;

	if (!Features::Noclip::Enabled)
	{
		Teardown::pGame->pPlayer->Velocity = exitVel;
	}
	else
	{
		newPos = Teardown::pGame->pPlayer->cameraPosition2;
	}
}

void Features::Noclip::doNoclip()
{
	if (!Enabled)
		return;

	if (Teardown::pGame->Status != Teardown::gameStatus::playing)
	{
		toggleNoclip();
		return;
	}

	Vector2 input = Teardown::pGame->pPlayer->moveKeys;

	if (Teardown::pGame->isPaused)
		return;

	glm::quat qXAxis = glm::angleAxis(Teardown::pGame->pPlayer->totalMouseDelta.x, glm::vec3(1.f, 0, 0));
	glm::quat qYAxis = glm::angleAxis(Teardown::pGame->pPlayer->totalMouseDelta.y, glm::vec3(0, 1.f, 0));

	newRot = qYAxis  * qXAxis;

	Teardown::pGame->pPlayer->cameraRotation2 = newRot;
	
	exitVel = Vector3(0, 0, 0);

	if (input.x != 0)
	{
		Vector3 fwdVec;

		fwdVec.x = 2 * (newRot.x * newRot.z + newRot.w * newRot.y);
		fwdVec.y = 2 * (newRot.y * newRot.z - newRot.w * newRot.x);
		fwdVec.z = 1 - 2 * (newRot.x * newRot.x + newRot.y * newRot.y);

		fwdVec *= input.x * (NoclipSpeed * Teardown::pGame->totalDelta);

		if (GetAsyncKeyState(VK_SHIFT))
			fwdVec *= 2.f;

		if (GetAsyncKeyState(VK_LCONTROL))
			fwdVec *= 0.25f;

		newPos -= fwdVec;
		exitVel -= fwdVec;
	}

	if (input.y != 0)
	{
		Vector3 sideVec;

		sideVec.x = 1 - 2 * (newRot.y * newRot.y + newRot.z * newRot.z);
		sideVec.y = 2 * (newRot.x * newRot.y + newRot.w * newRot.z);
		sideVec.z = 2 * (newRot.x * newRot.z - newRot.w * newRot.y);

		sideVec *= input.y * (NoclipSpeed * Teardown::pGame->totalDelta);

		if (GetAsyncKeyState(VK_SHIFT))
			sideVec *= 2.f;

		if (GetAsyncKeyState(VK_LCONTROL))
			sideVec *= 0.25f;

		newPos += sideVec;
		exitVel += sideVec;
	}

	exitVel *= Globals::FPS;

	Teardown::pGame->pPlayer->cameraPosition2 = newPos;
	Teardown::pGame->pPlayer->cameraPositionProcessed = newPos;

	Teardown::pGame->pPlayer->Position = newPos - heightOffset;
}