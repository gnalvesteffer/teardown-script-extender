#pragma once
#include "Vector.h"
#include <cstdint>

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
	Vector3 bodyPos; //0x0028
	Vector4 bodyRot; //0x0034
	Vector3 bodyPos2; //0x0044
	Vector4 bodyRot2; //0x0050
	Vector3 bodyPos3; //0x0060
	Vector4 bodyRot3; //0x006C
	Vector3 Velocity; //0x007C
	Vector3 rotForce; //0x0088
	char pad_01[72]; //0x0094
	Vector3 posMin; //0x00DC
	Vector3 posMax; //0x00E8
	bool dynamic; //0x00F4
	char pad_02[11]; //0x00F5
}; // Size: 0x100

class Shape : public Entity
{
public:
	char pad_0028[28];	//0x0028
	Vector3 posMax;		//0x0044
	Vector3 posMin;		//0x0050
	char pad_005C[52];	//0x005C
	Vox *pVox;			//0x0090
	char pad_0098[20];	//0x0098
	bool isBroken;		//0x00AC
	char pad_00AD[3];	//0x00AD
}; // Size: 0xB0

class Player
{
public:
	Vector3 position;
	Vector4 vehicleRotation;
	Vector3 unkVelocity;
	char pad_01[16];
	Vector3 velocity;
	char pad_02[12];
	Vector3 viewmodelVel;
	int awakeBodies;
	Vector3 cameraPosition;
	Vector4 cameraRot;
	Vector3 cameraPosition2;
	Vector4 cameraRot2;
	Vector3 cameraPosition3;
	Vector4 cameraRot3;
	float fwMovementDelta;
	float sdMovementDelta;
	float totalTravelDist;
	float travelDistDelta;
	Vector2 camDelta;
	Vector2 camDelta2;
	Vector2 moveKeys;
	Vector2 mouseDelta;
	char pad_03[1];
	bool isCrouching;
	char pad_04[1];
	bool isMouse2Down;
	char pad_05[1];
	bool isMouse1Down;
	char pad_06[2];
	float playerHeight;
	float camHeight;
	float baseSize;
	float unkSize;
	float playerSize;
	float unkSize2;
	float minHeight;
	float unkSize3;
	float crouchCompletion;
	float unkSize4;
	char pad_07[68];
	bool throwing;
	char pad_08[3];
	float health;
	char pad_09[616];
	int8_t equippedTool;
	char pad_10[7];
	class Body *toolBody;
	char pad_11[28];
	Vector3 toolPosition;
	Vector4 toolQuat;
	char pad_12[8];
	float recoil;
	float equipAnimation;
	float attackCooldown;
	float isOnGround;
	float airTravelTime;
	float isOnGround2;
	float airTravelTime2;
	char pad_13[12];
	Vector3 lastCollisionPos;
	char pad_14[16];
	Vector3 groundColour; // R, G, B (0.f - 1.f)
	char pad_15[20];
	bool isStandingOnDecal; // Only counts paint and burn marks
	char pad_16[7];
	float timeSinceLastJump;
	char pad_17[4];
	float viewPunchSlow;
	float viewPunchFast;
	Vector3 dragDelta; // not sure about this one
	Shape* pTargetShape;
	float eyeRaycastDist;
	Vector3 eyeRaycastPos;
	Vector3 unkDragRelated2;
	bool showGrabCircle;
	char pad_18[3];
	Shape* pGrabbedShape;
	Shape* pInteractableShape;
	char pad_19[52];
	float jumpHeight;
	char pad_20[24];
	float elapsedTime;
};