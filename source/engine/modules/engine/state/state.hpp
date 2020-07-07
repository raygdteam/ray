#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>

#include <core/module/module.hpp>

#include <engine/engine/engine.hpp>

struct IRayState
{
	/* core */
	
	ModuleManager* ModuleManager;

	/* engine */
	ray::IEngine* Engine;
};

RAY_ENGINE_API IRayState* RayState();
