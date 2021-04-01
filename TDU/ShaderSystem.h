#pragma once
#include "Types.h"

class ShaderSystem
{
public:
	char pad_0000[16]; //0x0000
	class Scene* pScene; //0x0010
	class visibleObjects* pVisible; //0x0018 Starts at shapes, + 0x98 counts lights
	class DistanceBlurShader* pDistanceBlurShader; //0x0020
	char pad_0028[8]; //0x0028
	class GameGLProgram* p3DGLProgram; //0x0030
	char pad_0038[12004]; //0x0038
	uint32_t* N00001E1B; //0x2F1C
	char pad_2F24[20380]; //0x2F24
	int32_t N00001664; //0x7EC0
	int32_t N000036FE; //0x7EC4
	char pad_7EC8[1976]; //0x7EC8
	Teardown::small_string renderBuffer; //0x8680
	bool drawBounds; //0x8698
	bool drawBodies; //0x8699
	bool drawShapes; //0x869A
	char pad_869B[1]; //0x869B
	bool drawShadowVolume; //0x869C
	char pad_869D[240]; //0x869D
}; //Size: 0x878D