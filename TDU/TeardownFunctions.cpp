#include "TeardownFunctions.h"
#include "Signatures.h"
#include "Memory.h"

void Teardown::Functions::GetAddresses()
{
	Teardown::Functions::Mem::GetAddresses();
	Teardown::Functions::Constructors::GetAddresses();
	Teardown::Functions::LuaFunctions::GetAddresses();
}