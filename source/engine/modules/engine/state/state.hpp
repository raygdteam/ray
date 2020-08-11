#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>

namespace ray
{
// Core
class RAY_CORE_API ModuleManager;
class RAY_CORE_API ObjectDb;
class RAY_CORE_API FileSystem;

// Engine
struct IEngine;

struct IRayState
{
	/* core */
	ModuleManager* ModuleManager;
	ObjectDb* ObjectDb;
	FileSystem* FileSystem;

	/* engine */
	IEngine* Engine;
};

RAY_ENGINE_API IRayState* RayState();
}
