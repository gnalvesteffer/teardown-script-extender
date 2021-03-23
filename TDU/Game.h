#pragma once
#include <cstdint>
#include "Entities.h"
#include "Types.h"

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
}

static const char* gameStatusStr[] = {
	"Splash Screen",
	"Menu",
	"UI Startup",
	"In-Game",
	"Editing",
	"Quitting"
};

class HUDShaderSettings
{
public:
	float UIXRescale; //0x0000
	float BTLSkew; //0x0004 Bottom to left skew
	float RTLCutoff; //0x0008 Right to left cutoff
	float MTLSkew; //0x000C Middle to left skew
	float BRSkew; //0x0010 Bottom right skew
	float UIYRescale; //0x0014
	float BTTClip; //0x0018 Bottom to top clip
	float BMSkew; //0x001C Bottom middle skew
	float cursorXOffset; //0x0020
	float cursorYOffset; //0x0024
	float N0000697C; //0x0028 Unknown, breaks cursor, values < -2 completely halt UI rendering
	float UIShrink; //0x002C
	float LTROffset; //0x0030 Left to right offset
	float TTBOffset; //0x0034 Top to bototm offset
	float N0000697E; //0x0038 Unknown, same behaviour as prev unknown value
	float UIScale; //0x003C
	float UIScaleX; //0x0040
	float RTBRSkew; //0x0044 Right to bottom right skew
	float RTLOffset; //0x0048 Right to left offset
	float LFTMSkew; //0x004C Left to top middle skew
	float BLTBRSkew; //0x0050 Bottom left to bottom right skew
	float UIYSize; //0x0054 Scale is relative to screen size
	float TTBClip; //0x0058 Top to bototm clip
	float BRTTLSkew; //0x005C Bottom right to top left skew
	float N0000698D; //0x0060 Unknown, doesn't seem to change anything
	float N00002B92; //0x0064 Unknown, doesn't seem to change anything
	float N00006993; //0x0068 Unknown, if you set it to 0 it makes you click on the last created elements
	float CursorYOffset; //0x006C
	float UIXOffset; //0x0070 In pixels
	float UIYOffset; //0x0074 Also in pixels
	float N00006996; //0x0078 Unknown, doesn't seem to change anything.
	float shrinkFromTopRight; //0x007C
}; //Size: 0x0080

class DistanceBlurShader
{
public:
	char pad_0000[4]; //0x0000
	float focusMax; //0x0004
	char pad_0008[4]; //0x0008
	bool hasInitialized; //0x000C
	char pad_000D[3]; //0x000D
	class GameGLProgram* p3DGLProgram; //0x0010
	char pad_0018[4440]; //0x0018
	int32_t programLocation; //0x1170
	char pad_1174[56]; //0x1174
	int32_t programLocation2; //0x11AC
	char pad_11B0[968]; //0x11B0
}; //Size: ?

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
	char pad_7EC8[1984]; //0x7EC8
	small_string renderBuffer; //0x8688
	bool drawBounds; //0x8698
	bool drawBodies; //0x8699
	bool drawShapes; //0x869A
	char pad_869B[1]; //0x869B
	bool drawShadowVolume; //0x869C
	char pad_869D[252]; //0x869D
}; //Size: ?

// IMPORTANT
// A lot of the arrays are inaccurate, they're dynamically scaled, first __int32 being the member count, second __int32 being the array size, and then there's a pointer to the array
class fireSystem
{
public:
	int32_t fireCount; //0x0008 limited to 200, if it goes over that number, it removes some fires to make sure it stays around that number
	int32_t highestFireCount; //0x000C
	class Fire(*pFireArray)[250]; //0x0010

	virtual void Function0();
	virtual void Function1();
}; //Size: 0x0018

class shapeSystem
{
public:
	int32_t shapeCount; //0x0000
	int32_t highestShapeCount; //0x0004
	Shape *(*pShapesArray)[8000]; //0x0008
}; //Size: 0x0010

class Debugging
{
public:
	bool showMenu; //0x0000
	char pad_0001[783]; //0x0001
	float avgFPS; //0x0310 Only updates when menu is rendering
}; //Size: ?

class gameSettings
{
public:
	int renderScale; //0x0000
	int renderQuality; //0x0004
	int gammaCorrection; //0x0008
	int FOV; //0x000C
	int motionBlur; //0x0010
	int barrelDistortion; //0x0014
	int DOF; //0x0018
	int VSync; //0x001C
	int soundVolume; //0x0020
	int ambienceVolume; //0x0024 (Unused?)
	int musicVolume; //0x0028
	int mouseSmoothing; //0x002C
	int mouseSensitivity; //0x0030
	int invertLook; //0x0034
	int headBob; //0x0038
}; //Size: 0x003C

class entityList
{
public:
	int32_t entCount; //0x0000
	int32_t arraySize; //0x0004
	class Entity(*pEntities)[16383]; //0x0008
}; //Size: 0x0010

class Scene
{
public:
	char pad_0008[8]; //0x0008
	class ppScene* ppScene; //0x0010
	char pad_0018[88]; //0x0018
	void* pParticleSystem; //0x0070
	char pad_0078[48]; //0x0078
	class fireSystem* pFireSystem; //0x00A8
	char pad_00B0[112]; //0x00B0
	class Vehicle* pCurrentVehicle; //0x0120
	class Screen* pCurrentScreen; //0x0128
	char pad_0130[248]; //0x0130
	entityList entityList; //0x0228
	char pad_0238[8]; //0x0238
	int32_t* pBodyCount; //0x0240
	class shapeList* pShapeList; //0x0248
	char pad_0250[10248]; //0x0250
	void* (*pSomethingArray)[150]; //0x2A58
	char pad_2A60[1728]; //0x2A60

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: ?

class LevelManager
{
public:
	char* levelName; //0x0000
	char pad_0008[7]; //0x0008
	bool hasPlayed; //0x000F
	char pad_0010[376]; //0x0010
}; //Size: ?

class UIGLProgram
{
public:
	int32_t mouseX; //0x0000
	int32_t mouseY; //0x0004
	int32_t mouseXDelta; //0x0008
	int32_t mouseYDelta; //0x000C
	char pad_0010[907]; //0x0010
	bool mouse1Down; //0x039B
	char pad_039C[1]; //0x039C
	bool mouse2Down; //0x039D
	char pad_039E[18]; //0x039E
	bool mouse1Down2; //0x03B0
	char pad_03B1[127]; //0x03B1
	int32_t lastClickX; //0x0430
	int32_t lastClickY; //0x0434
	char pad_0438[248]; //0x0438
	int32_t lastClickX2; //0x0530
	int32_t lastClickY2; //0x0534
	char pad_0538[248]; //0x0538
	int32_t lastClickX3; //0x0630
	int32_t lastClickY3; //0x0634
	char pad_0638[1688]; //0x0638
}; //Size: ?

class GameGLProgram
{
public:
	char pad_0000[280]; //0x0000
	int32_t resX; //0x0118
	int32_t resY; //0x011C
	char pad_0120[655]; //0x0120
	int32_t unknownLocation; //0x03AF
	char pad_03B3[88]; //0x03B3
}; //Size: ?

class Game
{
public:
	char pad_0000[8]; //0x0000
	int8_t Status; //0x0008 6: quitting | 5: editor | 4: in game | 3: starting ui | 2: in menu | 1: splash
	char pad_0009[3]; //0x0009
	int8_t statusTransition; //0x000C
	char pad_000D[3]; //0x000D
	float loadingFade; //0x0010
	bool isPlaying; //0x0014
	char pad_0015[3]; //0x0015
	float loadingFade2; //0x0018
	bool isMapOpen; //0x001C
	char pad_001D[11]; //0x001D
	GameGLProgram * pGameGLProgram; //0x0028 No idea if they're programs, devices, or whatever, I'd love to know though
	UIGLProgram * pUIGLProgram; //0x0030 cause i seriously don't know anything regarding this
	ShaderSystem* pShaderSystem; //0x0038
	class Scene* pScene; //0x0040
	class SoundSystem* pSoundSystem; //0x0048
	class CameraSystem* pCameraSystem; //0x0050
	class EditorUndoable* pEditorUndoable; //0x0058
	char pad_0060[8]; //0x0060
	void* pHud; //0x0068
	void* pHud2; //0x0070
	void* pHud3; //0x0078
	class HUD* pHud4; //0x0080
	Debugging* pDebugging; //0x0088
	class Device* pDevice; //0x0090
	Player* pPlayer; //0x0098
	class LevelManager* pLevelManager; //0x00A0
	char pad_00A8[16]; //0x00A8
	void* pData; //0x00B8
	class modMgr* pModMgr; //0x00C0
	char pad_00C8[72]; //0x00C8
	bool isPaused; //0x0110
	char pad_0111[3]; //0x0111
	int32_t levelTime; //0x0114
	int32_t playTime; //0x0118
	char pad_011C[4]; //0x011C
	float timeScale; //0x0120
	float timeStep; //0x0124 1 / 59-ish = default val
	float totalDelta; //0x0128
	float totalTime; //0x012C
	float gameTime; //0x0130
	float gameDelta; //0x0134
	char pad_0138[156]; //0x0138
	HUDShaderSettings HUDShaderSettings; //0x01D4
	bool isLoadingSave; //0x0254
	bool isSaving; //0x0255
	bool applyGraphics; //0x0256
	bool respawnPlayer; //0x0257
	float weirdEffect; //0x0258
	char pad_025C[24]; //0x025C
	gameSettings gameSettings; //0x0274
	char pad_02B0[24]; //0x02B0
	small_string levelXMLPath; //0x02C8
	char pad_02D0[88]; //0x02D0
	char* levelXMLPath2; //0x0330
	char pad_0338[80]; //0x0338
	small_string currentlyEditingXMLPath; //0x0388
	bool canReturnToEditor; //0x0398
	char pad_0399[7]; //0x0399
	char* editorTempXML; //0x03A0
	char pad_03A8[9]; //0x03A8
	bool updatingPlayer; //0x03B1
	char pad_03B2[22]; //0x03B2
	int32_t finalGLProgram; //0x03C8 Final renderTarget
	char pad_03CC[40]; //0x03CC
	int32_t screenResX; //0x03F4
	int32_t screenResY; //0x03F8
	char pad_03FC[20]; //0x03FC
}; //Size: 0x0410 / Verified