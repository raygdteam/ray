#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>

class RAY_CORE_API FileSystem;
class RAY_CORE_API ObjectDb;
class RAY_CORE_API Debug;

namespace ray
{
// Core
class RAY_CORE_API ModuleManager;

// Engine
struct IEngine;

struct IRayState
{
	/* core */
	ModuleManager* ModuleManager;
	ObjectDb* ObjectDb;
	FileSystem* FileSystem;
	Debug* Debug;

	/* engine */
	IEngine* Engine;
};

RAY_ENGINE_API IRayState* RayState();
}
