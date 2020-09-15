#pragma once
#include <core/core.hpp>
#include <engine/engine/engine_def.hpp>
#include <input/input_def.hpp>

// Core
class RAY_CORE_API FileSystem;
class RAY_CORE_API ObjectDb;
class RAY_CORE_API Debug;
class RAY_CORE_API ModuleManager;

// Input
class RAY_INPUT_API Input;

// Engine
struct IEngine;
class RAY_ENGINE_API ResourceManager;

struct IRayState
{
	/* core */
	ModuleManager* ModuleManager;
	ObjectDb* ObjectDb;
	FileSystem* FileSystem;
	Debug* Debug;

	/* input */
	Input* Input;

	/* engine */
	IEngine* Engine;
	ResourceManager* ResourceManager;
};

RAY_ENGINE_API IRayState* RayState();
