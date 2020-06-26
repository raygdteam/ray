#include "core/core.hpp"
#include "engine_loop.hpp"
#include <engine/engine/engine.hpp>

int RayMain()
{
	ray::launcher::EngineLoop engineLoop;
	engineLoop.PreInitialize();
	engineLoop.Initialize();

	if(ray::IsEngineExitRequested())
	{
		// TODO: xxx
	}

	while (!ray::IsEngineExitRequested())
		engineLoop.Tick();


	return 0;
}
