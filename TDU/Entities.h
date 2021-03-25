#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

// Entity name	| Hex size	| Size in bytes	| Entity ID
// Body			|	0xE8	|	232 bytes	|	1
// Shape		|	0xB0	|	176 bytes	|	2
// Light		|	0x1360	|	4960 bytes	|	3
// Location		|	0x48	|	72 bytes	|	4
// Water		|	0x370	|	880 bytes	|	5
// Enemy		|	0x440	|	1088 bytes	|	6
// Joint		|	0xC8	|	200 bytes	|	7
// Vehicle		|	0x288	|	648 bytes	|	8
// Wheel		|	0xF0	|	240 bytes	|	9
// Screen		|	0xC78	|	3192 bytes	|	10
// Trigger		|	0x110	|	272 bytes	|	11
// Script		|	0xAC8	|	2760 bytes	|	12
/////////////////////////////////////////////////////////
//	//	//	//	//	//	Other stuff	//	//	//	//	//	/
/////////////////////////////////////////////////////////
//	Entity		|	0x28	|	40 bytes
//	Vox			|	UNK		|	unknown size
//	Convex		|	0x2B8	|	696 bytes
//	HUD			|	0xA90	|	2704 bytes

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

class Entity
{
public:
	entityType	Type;			//0x08
	int8_t		Flags;			//0x09
	char		pad_000A[2]; //0x000A
	int16_t		Id; //0x000C
	char		pad_000E[2];	//0x0E
	Entity*		pParent;		//0x10
	Entity*		pSibling;		//0x18
	Entity*		pChild;			//0x20

	virtual void	Destroy(Entity* entity, bool freeMemory);
	virtual Entity* SetParent(Entity* entity, Entity* parent);
	virtual void	Function2();
	virtual void	Function3();
}; // Size: 0x28

class Shape : public Entity
{
public:
	char pad_0028[28]; //0x0028
	glm::vec3 posMin; //0x0044
	glm::vec3 posMax; //0x0050
	char pad_005C[4]; //0x005C
	void* pDormantSomething; //0x0060
	void* pActiveSomething; //0x0068
	char pad_0070[32]; //0x0070
	class Vox* pVox; //0x0090
	char pad_0098[8]; //0x0098
	void* pJoints; //0x00A0 
	float Intergrity; //0x00A8
	bool isBroken; //0x00AC
	char pad_00AD[3]; //0x00AD
}; //Size: 0xB0

class Body : public Entity
{
public:
	glm::vec3 Position; //0x0028
	glm::quat Rotation; //0x0034
	glm::vec3 Position2; //0x0044
	glm::quat Rotation2; //0x0050
	glm::vec3 Position3; //0x0060
	glm::quat Rotation3; //0x006C
	glm::vec3 Velocity; //0x007C
	glm::vec3 RotationVelocity; //0x0088
	char pad_0094[12]; //0x0094
	float Lightness; //0x00A0
	char pad_00A4[24]; //0x00A4
	float Integrity; //0x00BC
	glm::vec3 N00006C83; //0x00C0
	glm::vec3 N00006C80; //0x00CC
	float N00002F17; //0x00D8
	bool Dynamic; //0x00DC
	char pad_00DD[8]; //0x00DD
	bool Colliding; //0x00E5
	char pad_00E6[2]; //0x00E6
}; //Size: 0xE8

class Vehicle : public Entity
{
public:
	Body* pBody; //0x0028
	glm::vec3 camPosition; //0x0030
	glm::quat rotation; //0x003C
	char pad_004C[40]; //0x004C
	int8_t wheelCount; //0x0074
	char pad_0075[199]; //0x0075
	glm::vec2 moveInput; //0x013C
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
	char pad_0285[3]; // 0x0285
}; // Size: 0x288