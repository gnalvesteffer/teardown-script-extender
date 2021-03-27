#include "CLuaFunctions.h"
#include "TeardownFunctions.h"
#include <Windows.h>

int CLuaFunctions::EntityFunctions::CreateBody(lua_State* L)
{
	int argCount = lua_gettop(L);
	DWORD64 parent = 0;
	if (argCount >= 1)
	{
		int parentId = luaL_checknumber(L, 1);
		parent = (DWORD64)Teardown::Functions::EntityFunctions::GetEntityById(parentId);
	}

	Body* newBody = Teardown::Functions::Constructors::newBody((Entity*)parent);

	lua_pushnumber(L, newBody->Id);
	return 1;
}

int CLuaFunctions::EntityFunctions::CreateShape(lua_State* L)
{
	int argCount = lua_gettop(L);
	Entity* parent = 0;
	if (argCount >= 1)
	{
		int parentId = luaL_checknumber(L, 1);
		parent = Teardown::Functions::EntityFunctions::GetEntityById(parentId);
	}

	Shape* newShape = Teardown::Functions::Constructors::newShape(parent);

	lua_pushnumber(L, newShape->Id);
	return 1;
}

int CLuaFunctions::EntityFunctions::LoadVox(lua_State* L)
{
	int shapeId = luaL_checknumber(L, 1);
	const char* voxPath = luaL_checkstring(L, 2);

	Shape* pShape = (Shape*)Teardown::Functions::EntityFunctions::GetEntityById(shapeId);

	if (!pShape || pShape->Type != entityType::Shape)
		return 0;

	Vox* newVox = Teardown::Functions::EntityFunctions::LoadVox(voxPath, 1.f);

	if (newVox)
		pShape->pVox = newVox;

	return 0;
}