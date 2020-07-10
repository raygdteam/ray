#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>

#include <core/module/module.hpp>
#include <core/object/object_db.hpp>

#include <engine/engine/engine.hpp>
#include <core/file_system/file_system.hpp>

struct IRayState
{
	/* core */
	ModuleManager* ModuleManager;
	ObjectDb* ObjectDb;
	ray::file_system::FileSystem* FileSystem;

	/* engine */
	ray::IEngine* Engine;
};

RAY_ENGINE_API IRayState* RayState();
