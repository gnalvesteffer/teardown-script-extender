#include "Teardown.h"

void Teardown::GetFunctionAddresses()
{
	Teardown::Functions::Data::GetFunctionAddresses();
	Teardown::Functions::Shaders::GetFunctionAddresses();
}