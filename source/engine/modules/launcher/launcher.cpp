#include "core/core.hpp"
#include "engine_loop.hpp"
#include <engine/engine/engine.hpp>
#include <core/log/log.hpp>

#include <core/memory/new_delete_override.hpp>

static Logger* gLog;

int RayMain()
{
	gLog = new Logger("launcher");
	ray::launcher::EngineLoop engineLoop;
	engineLoop.PreInitialize();
	engineLoop.Initialize();

	if (ray::IsEngineExitRequested())
	{
		gLog->Log("engine early exit");
		return 1;
	}

	while (!ray::IsEngineExitRequested())
		engineLoop.Tick();

	return 0;
}