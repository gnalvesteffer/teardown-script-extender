#include "Teardown.h"
#include "Shaders.h"
#include "Functions.h"

// To-do:
//		Expose more internal functions
void Teardown::Functions::getFunctionAddresses()
{
	Teardown::Functions::MiscFunctions::getFunctionAddresses();
	Teardown::Shaders::getFunctionAddresses();
}