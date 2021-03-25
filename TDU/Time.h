#include <chrono>

/*
	Made this to not have to type that whole thing everytime i wanna get the time
	Unfinished, it's missing a lot
*/

namespace Time
{
	inline uint64_t timeAsMilliseconds()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}
}