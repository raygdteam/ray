#include "core/core.hpp"
#include "engine_loop.hpp"
#include <engine/engine/engine.hpp>
#include <core/log/log.hpp>

#include <core/memory/new_delete_override.hpp>

static ray::core::log::logger* gLog;

int RayMain()
{
	gLog = new ray::core::log::logger("launcher");
	EngineLoop engineLoop;
	engineLoop.PreInitialize();
	engineLoop.Initialize();

	if (IsEngineExitRequested())
	{
		gLog->Log("engine early exit");
		return 1;
	}

	while (!IsEngineExitRequested())
		engineLoop.Tick();

	return 0;
}