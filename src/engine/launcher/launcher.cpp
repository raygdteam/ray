#include "core/core.hpp"
#include "engine_loop.hpp"
#include <engine/engine/engine.hpp>

int RayMain(pcwstr cmd, s32 argc)
{
	__asm
	{
		mov ax, ax
	}

	ray::launcher::EngineLoop engineLoop;
	engineLoop.PreInitialize(cmd, argc);
	engineLoop.Initialize();
	
	if(ray::IsEngineExitRequested())
	{
		// TODO: xxx
	}

	while (!ray::IsEngineExitRequested())
		engineLoop.Tick();

	return 0;
}
