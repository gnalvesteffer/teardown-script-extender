#pragma once
#include "Types.h"
#include "Entities.h"

// Created with ReClass.NET 1.2 by KN4CK3R

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
	Vehicle* pCurrentVehicle; //0x0120
	class Screen* pCurrentScreen; //0x0128
	char pad_0130[248]; //0x0130
	class entityList entityList; //0x0228
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
}; //Size: 0x3120