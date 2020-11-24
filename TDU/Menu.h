#pragma once
namespace Menu
{
	extern bool drawMenu;

	extern bool overrideFog;
	extern float fogColour[4];

	extern bool overrideAmbient;
	extern float ambientColourAvg[4];

	extern bool overrideWater;
	extern float wave;
	extern float ripple;
	extern float motion;
	extern float foam;

	extern bool overrideDOF;
	extern float focusMin;
	extern float focusMax;

	extern bool overrideWetness;
	extern float wetness;
	extern float puddleAmount;
	extern float puddleFreq;

	void Draw();
	void Init();
}