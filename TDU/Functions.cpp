#include "Teardown.h"
#include "Functions.h"

void Teardown::Functions::getFunctionAddresses()
{
	Teardown::Functions::Shaders::getFunctionAddresses();
	Teardown::Functions::Entities::getFunctionAddresses();
}