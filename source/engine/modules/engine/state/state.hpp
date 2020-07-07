#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>

#include <core/module/module.hpp>
#include <core/object/object_db.hpp>

#include <engine/engine/engine.hpp>

struct IRayState
{
	/* core */
	ModuleManager* ModuleManager;
	ObjectDb* ObjectDb;

	/* engine */
	ray::IEngine* Engine;
};

RAY_ENGINE_API IRayState* RayState();
