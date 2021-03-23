#pragma once
#include "Vector.h"
#include "Types.h"

#include <cstdint>

#include <glm/gtc/quaternion.hpp>

// Entity total sizes: ( Not accounting for class inheritance )
// Body:		0x100	/	256 bytes
// Shape:		0xB0	/	176 bytes
// Light:		0x1358	/	4952 bytes
// Location:	0x48	/	72 bytes
// Water:		0x370	/	880 bytes
// Enemy:		0x440	/	1088 bytes
// Joint:		0xC8	/	200 bytes
// Vehicle:		0x288	/	648 bytes
// Wheel:		0xF0	/	240 bytes
// Screen:		0xC78	/	3192 bytes
// Trigger:		0x110	/	272 bytes
// Script:		0x200	/	512 bytes

// Other sizes:
// Entity:		0x28	/	40 bytes
// Vox:			0x60	/	96 bytes
// Convex:		0x2B8	/	696 bytes
// HUD:			0x3DA8	/	15784 bytes

enum class entityType : int8_t
{
	Body = 1,
	Shape = 2,
	Light = 3,
	Location = 4,
	Water = 5,
	Enemy = 6,
	Joint = 7,
	Vehicle = 8,
	Wheel = 9,
	Screen = 10,
	Trigger = 11,
	Script = 12
};

static const char* entityTypeStr[] = {
	"Body",
	"Shape",
	"Light",
	"Location",
	"Water",
	"Enemy",
	"Joint",
	"Vehicle",
	"Wheel",
	"Screen",
	"Trigger",
	"Script"
};

class Vox
{
public:
	uint32_t m_BufferSize[3];		// 0x8
	uint8_t padC[4];				// 0x14
	uint8_t* m_MaterialBuffer;		// 0x18
	uint8_t* m_PhysicsBuffer;		// 0x19
	float m_Scale;					// 0x1A
	uint16_t m_TextureSize[3];		// 0x1E
	uint64_t* qword2A;				// 0x24
	uint8_t gap32[6];				// 0x2C
	uint64_t* m_BindlessTexture;	// 0x34
	uint32_t m_Texture;				// 0x38
	Vector3 m_Unk;					// 0x44
	uint32_t m_Palette;				// 0x48
	uint8_t byte54;					// 0x49
	uint8_t pad55[3];				// 0x4A
	uint32_t m_VoxelCount;			// 0x4D
	uint8_t pad5C[4];				// 0x51

	virtual void Function01();
	virtual void Function02();
	virtual void Function03();
	virtual void Function04();
	virtual void Function05();
	virtual void Function06();
}; // Size: 0x60

class Tool
{
public:
	char* modelPath; //0x0000
	int32_t unk; //0x0008
	char pad_000C[4]; //0x000C
	float modelRecoil; //0x0010
	float cameraRecoil; //0x0014
	Vector3 Position; //0x0018 Relative to camera / player
	Vector4 Rotation; //0x0024
	float cooldown; //0x0034
}; //Size: 0x0038

class Entity
{
public:
	entityType	Type;			//0x08
	int8_t		Flags;			//0x09
	int32_t		ID;				//0x0A
	char		pad_000E[2];	//0x0E
	Entity*		pParent;		//0x10
	Entity*		pSibling;		//0x18
	Entity*		pChild;			//0x20

	virtual void	destroy(Entity* entity, bool freeMem);
	virtual Entity* setParent(Entity* entity, Entity* parent);
	virtual void	Function2();
	virtual void	Function3();
}; // Size: 0x28

class Body : public Entity
{
public:
	Vector3 Position; //0x0028
	Vector4 Rotation; //0x0034
	Vector3 Position2; //0x0044
	Vector4 Rotation2; //0x0050
	Vector3 Position3; //0x0060
	Vector4 Rotation3; //0x006C
	Vector3 Velocity; //0x007C
	Vector3 rotationVelocity; //0x0088
	char pad_0094[12]; //0x0094
	float Lightness; //0x00A0
	char pad_00A4[24]; //0x00A4
	float Integrity; //0x00BC Goes from 1 straight to 0, no in-between, similar to the isBroken bool in Shape but it's a float
	char pad_00C0[28]; //0x00C0
	Vector3 posMin; //0x00DC
	Vector3 posMax; //0x00E8
	bool dynamic; //0x00F4
	char pad_00F5[3]; //0x00F5
}; //Size: 0x00F8

class Shape : public Entity
{
public:
	char pad_0028[28]; //0x0028
	Vector3 posMin; //0x0044
	Vector3 posMax; //0x0050
	char pad_005C[52]; //0x005C
	class Vox *pVox; //0x0090
	char pad_0098[16]; //0x0098
	float Integrity; //0x00A8
	bool isBroken; //0x00AC
	char pad_00AD[19]; //0x00AD
}; //Size: 0x00C0 (Seems about right, unverified)

class Fire
{
public:
	class Shape *pShape; //0x0000
	char pad_0008[16]; //0x0008
	float lifeTime; //0x0018 Every fire dies after hitting 10s
	bool unk; //0x001C
	bool hasBurnedSource; //0x001D
	char pad_001E[10]; //0x001E
}; //Size: 0x0028 (Seems about right, unverified)

class Vehicle : public Entity
{
public:
	Body *pBody; //0x0028
	Vector3 camPosition; //0x0030
	Vector4 rotation; //0x003C
	char pad_004C[40]; //0x004C
	int8_t wheelCount; //0x0074
	char pad_0075[199]; //0x0075
	Vector2 moveInput; //0x013C
	float handbrake; //0x0144
	float mouse1Down; //0x0148
	float mouse2Down; //0x014C
	char pad_0150[8]; //0x0150
	float wheelTurn; //0x0158
	float accelCompletion; //0x015C
	char pad_0160[4]; //0x0160
	float accelCompletion2; //0x0164
	float N000014F4; //0x0168
	float vehicleCondition; //0x016C
	char pad_0170[276]; //0x0170
	bool isBraking; //0x0284
};

class Player
{
public:
	Vector3 Position; //0x0000
	Vector4 vehicleRotation; //0x000C
	Vector3 unknown; //0x001C
	char pad_0028[16]; //0x0028
	Vector3 Velocity; //0x0038
	char pad_0044[12]; //0x0044
	Vector3 Velocity2; //0x0050
	int32_t lastInteractedShapeIdx; //0x005C
	Vector3 cameraPositionProcessed; //0x0060
	glm::quat cameraRotationProcessed; //0x006C
	Vector3 cameraPosition2; //0x007C
	glm::quat cameraRotation2; //0x0088
	Vector3 cameraPosition3; //0x0098
	glm::quat cameraRotation3; //0x00A4
	float fwdDelta; //0x00B4
	float sideDelta; //0x00B8
	float travelDist; //0x00BC
	float travelDist2; //0x00C0 resets after hitting 1
	Vector2 totalProcessedMouseDelta; //0x00C4
	Vector2 totalMouseDelta; //0x00CC
	Vector2 moveKeys; //0x00D4
	Vector2 mouseDelta; //0x00DC
	char pad_00E4[1]; //0x00E4
	bool isCrouched; //0x00E5
	char pad_00E6[1]; //0x00E6
	bool isMouse2Down; //0x00E7
	char pad_00E8[1]; //0x00E8
	bool isMouse1Down; //0x00E9
	char pad_00EA[2]; //0x00EA
	float playerHeight; //0x00EC
	char pad_00F0[28]; //0x00F0
	float crouchCompletion; //0x010C
	char pad_0110[8]; //0x0110
	class Body* pGrabbedBody; //0x0118
	char pad_0120[40]; //0x0120
	float grabDist1; //0x0148
	float grabDist2; //0x014C
	class Body* pGrabbedBody2; //0x0150
	bool isThrowing; //0x0158
	char pad_0159[3]; //0x0159
	float Health; //0x015C
	char pad_0160[56]; //0x0160
	Tool sledge; //0x0198
	Tool spraycan; //0x01D0
	Tool extinguisher; //0x0208
	Tool blowtorch; //0x0240
	Tool Shotgun; //0x0278
	Tool Plank; //0x02B0
	Tool Pipebomb; //0x02E8
	Tool Gun; //0x0320
	Tool Bomb; //0x0358
	Tool Rocket; //0x0390
	small_string equippedToolName; //0x03C8
	class Body* pToolBody; //0x03D8
	char pad_03E0[28]; //0x03E0
	Vector3 toolPos; //0x03FC
	Vector4 toolRot; //0x0408
	char pad_0418[8]; //0x0418
	float recoil; //0x0420
	float equipAnimation; //0x0424 Something related to equipping
	float toolCooldown; //0x0428
	float onGround; //0x042C
	float airTime; //0x0430
	float onGround2; //0x0434
	float airTime2; //0x0438
	Vector3 groundNormal; //0x043C Uncertain
	Vector3 lastWalkedPos; //0x0448
	char pad_0454[4]; //0x0454
	class Body* pLastCollidedBody; //0x0458
	int32_t groundSurfaceId; //0x0460
	float groundR; //0x0464
	float groundG; //0x0468
	float groundB; //0x046C
	char pad_0470[20]; //0x0470
	bool standingOnDecal; //0x0484 I know, there are no decals, but palettes, I couldn't come up with a better name
	char pad_0485[7]; //0x0485
	float timeSinceLastJump; //0x048C
	char pad_0490[24]; //0x0490
	class Shape* pTargetShape; //0x04A8
	float raycastDist; //0x04B0
	Vector3 raycastHit; //0x04B4
	char pad_04C0[12]; //0x04C0
	bool canGrab; //0x04CC
	char pad_04CD[3]; //0x04CD
	class Shape* pGrabbedShape; //0x04D0
	class Shape* pInteractableShape; //0x04D8
	char pad_04E0[52]; //0x04E0
	float jumpHeight; //0x0514
	char pad_0518[8]; //0x0518
	class Water* pSwimmingWater; //0x0520
	float underwaterDepth; //0x0528 Only updates when player is touching water
	bool isUnderwater; //0x052C
	char pad_052D[3]; //0x052D
	float playedTime2; //0x0530 Starts at 3, counts total play time (resets on restart / level load)
	float playedTime; //0x0534
	float underwaterTime; //0x0538 Starts drowning at 10s
	char pad_053C[5]; //0x053C
	bool canUseTool; //0x0541
	char pad_0542[2304]; //0x0542
}; //Size: 0x0E42