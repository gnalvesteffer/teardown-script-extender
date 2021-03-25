#include "CLuaFunctions.h"
#include "Config.h"

void CLuaFunctions::RegisterCFunctions(lua_State* L)
{
	#if ENABLE_DEBUG_LIB
		luaopen_debug(L);
	#endif

	#if ENABLE_OS_LIB
		luaopen_os(L);
	#endif

	#if ENABLE_IO_LIB
		luaopen_io(L);
	#endif
}