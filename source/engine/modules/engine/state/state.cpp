#include "state.hpp"
#include <cstring>

static IRayState* gState = nullptr;

IRayState* RayState()
{
	if (gState == nullptr)
	{
		gState = new IRayState();
		memset(gState, 0, sizeof(IRayState));
	}

	return gState;
}
