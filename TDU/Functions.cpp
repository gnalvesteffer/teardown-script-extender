#include "Teardown.h"
#include "Shaders.h"

// To-do:
//		Expose more internal functions
void Teardown::GetFunctionAddresses()
{
	Teardown::Shaders::getFunctionAddresses();
}