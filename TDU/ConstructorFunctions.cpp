#include "TeardownFunctions.h"
#include "Signatures.h"
#include "Memory.h"
#include "Globals.h"
#include "Logger.h"
#include "Teardown.h"

#include "Entities.h"
#include "Script.h"
#include "Types.h"

typedef Entity* (*tEntityCCtor)				(void* memAlloc, Entity* Parent);

tEntityCCtor BodyCCtor;
tEntityCCtor ShapeCCtor;
tEntityCCtor LightCCtor;
tEntityCCtor LocationCCtor;
tEntityCCtor WaterCCtor;
tEntityCCtor EnemyCCtor;
tEntityCCtor JointCCtor;
tEntityCCtor VehicleCCtor;
tEntityCCtor WheelCCtor;
tEntityCCtor ScreenCCtor;
tEntityCCtor TriggerCCtor;
tEntityCCtor ScriptCCtor;

Script* Teardown::Functions::Constructors::newScript(Entity* Parent)
{
	void* memAlloc = Teardown::Functions::Mem::Alloc(sizeof(Script));
	return (Script*)ScriptCCtor(memAlloc, Parent);
}

Body* Teardown::Functions::Constructors::newBody(Entity* Parent)
{
	void* memAlloc = Teardown::Functions::Mem::Alloc(sizeof(Body));
	return (Body*)BodyCCtor(memAlloc, Parent);
}

Shape* Teardown::Functions::Constructors::newShape(Entity* Parent)
{
	void* memAlloc = Teardown::Functions::Mem::Alloc(sizeof(Shape));
	return (Shape*)ShapeCCtor(memAlloc, Parent);
}

void Teardown::Functions::Constructors::GetAddresses()
{
	DWORD64 dwBodyCCtor = Memory::FindPattern(Signatures::ClassConstructors::Body.pattern, Signatures::ClassConstructors::Body.mask, Globals::HModule);
	BodyCCtor = (tEntityCCtor)Memory::readPtr(dwBodyCCtor, 1);
	
	DWORD64 dwShapeCCtor = Memory::FindPattern(Signatures::ClassConstructors::Shape.pattern, Signatures::ClassConstructors::Shape.mask, Globals::HModule);
	ShapeCCtor = (tEntityCCtor)Memory::readPtr(dwShapeCCtor, 1);

	DWORD64 dwScriptCCtor = Memory::FindPattern(Signatures::ClassConstructors::Script.pattern, Signatures::ClassConstructors::Script.mask, Globals::HModule);
	ScriptCCtor = (tEntityCCtor)Memory::readPtr(dwScriptCCtor, 1);

	WriteLog(LogType::Address, "Body::Body: 0x%p", BodyCCtor);
	WriteLog(LogType::Address, "Shape::Shape: 0x%p", ShapeCCtor);
	WriteLog(LogType::Address, "Script::Script: 0x%p", ScriptCCtor);
}