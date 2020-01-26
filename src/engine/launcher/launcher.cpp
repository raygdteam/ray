#include "core/core.hpp"
#include "engine_loop.hpp"

int RayMain(pcwstr cmd, s32 argc)
{
	ray::launcher::EngineLoop engineLoop;
	engineLoop.PreInitialize(cmd, argc);
	return 0;
}
