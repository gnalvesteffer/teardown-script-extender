#pragma once
#include <cstdint>
#include "Entities.h"
#include "Types.h"

enum gameStatus : int8_t
{
	splashScreen = 1,
	menu = 2,
	about = 3,
	uistart = 4,
	playing = 5,
	editor = 6,
	quitting = 7
};

static const char* gameStatusStr[] = {
	"Splash Screen",
	"Menu",
	"About",
	"UI Startup",
	"In-Game",
	"Editing",
	"Quitting"
};

class ShaderSystem
{
public:
	char pad_0000[24];
	class visibleObjects *pVisible;
	char pad_0020[31280];
	small_string renderTarget;
	bool drawBounds;
	bool drawBodies;
	bool unkbool;
	bool drawShadowVolume;
}; // Size: ?




class Settings
{
public:
	int renderScale; //0x0000
	int renderQuality; //0x0004
	int gammaCorrection; //0x000C
	int FOV; //0x0010
	int motionBlurEnabled; //0x0014
	int barrelDistortionEnabled; //0x0018
	int vSyncType; //0x001C
	int soundVolume; //0x0020
	int ambienceVolume; //0x0024
	int musicVolume; //0x0028
	int smoothing; //0x002C
	int sensitivity; //0x0030
	int invert; //0x0034
	int headbob; //0x0038
}; //Size: 0x003C

class Game
{
public:
	char pad_0000[8]; //0x0000
	int8_t Status; //0x0008 7: quitting | 6: editor | 5: in game | 4: starting ui | 3: in about | 2: in menu | 1: splash
	char pad_0009[3]; //0x0009
	int8_t statusTransition; //0x000C
	char pad_000D[3]; //0x000D
	float loadingFade; //0x0010
	bool isPlaying; //0x0014
	char pad_0015[3]; //0x0015
	float loadingFade2; //0x0018
	char pad_001C[28]; //0x001C
	ShaderSystem *pShaderSystem; //0x0038
	class Scene *pScene; //0x0040
	char pad_0048[8]; //0x0048
	class CameraSystem *pCameraSystem; //0x0050
	class EditorUndoable *pEditorUndoable; //0x0058
	char pad_0060[16]; //0x0060
	void *pHud; //0x0070
	void *pHud2; //0x0078
	void *pHud3; //0x0080
	void *pHud4; //0x0088
	void *pUnknown; //0x0090
	class Device *pDevice; //0x0098
	class Player *pPlayer; //0x00A0
	char *currentLevel; //0x00A8 Stores the name of the original level when playing sandbox
	char pad_00B0[16]; //0x00B0
	void *pValues; //0x00C0
	char pad_00C8[48]; //0x00C8
	char *currentLevelFile; //0x00F8
	char pad_0100[8]; //0x0100
	char *currentLevelId; //0x0108 Does not work on create levels
	char pad_0110[8]; //0x0110
	char *currentLevelFlag; //0x0118
	char pad_0120[8]; //0x0120
	bool levelUnk2; //0x0128
	char pad_0129[15]; //0x0129
	bool isPaused; //0x0138
	char pad_0139[11]; //0x0139
	float unknown; //0x0144
	float timeDelta; //0x0148
	float totalElapsedTime; //0x014C
	float elapsedTime; //0x0150
	float timeDelta2; //0x0154
	char pad_0158[8]; //0x0158
	int32_t tickCount; //0x0160
	char pad_0164[272]; //0x0164
	bool isQuickLoading; //0x0274
	bool isQuickSaving; //0x0275
	bool isRestarting; //0x0276
	char pad_0277[33]; //0x0277
	class Settings gameSettings; //0x0298
	char pad_02D0[116]; //0x02D0
}; //Size: 0x0344