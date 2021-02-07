//#define LAUNCH_EDITOR 1

#include "engine_loop.hpp"
#include <core/core.hpp>

#include <engine/state/state.hpp>

#include <core/object/object.hpp>
#include <core/module/module.hpp>
#include <core/file_system/file_system.hpp>
#include <core/debug/debug.hpp>
#include <core/threading/thread_pool.hpp>

#include <input/input.hpp>

#include <resources/resource_manager.hpp>

#ifndef RAY_RELEASE
#include <editor/engine/engine_interface.hpp>
#endif

extern Logger* gLauncherLog;

void EngineLoop::PreInitialize()
{
	gLauncherLog->Log("------- Beginning of EngineLoop");
	
	/* 1. Initialize RayState and vital core components. */
	/* First call to RayState() will allocate the memory. */
	IRayState* state = RayState();
	state->ObjectDb = new ObjectDb();
	state->ModuleManager = new ModuleManager();
	state->FileSystem = new FileSystem();
	state->Debug = new Debug();
	state->Input = new Input();
	state->ResourceManager = new ResourceManager(state);

	gThreadPoolManager = new ThreadPoolManager;

	if (
#ifndef RAY_RELEASE
		/*strcmp(GetCommandLineA(), "-editor") != 0
		|| strcmp(GetCommandLineA(), "-project=") == 0*/ false
#else
		false
#endif
		)
	{
		gLauncherLog->Log("------- Beginning of EditorEngine");

		auto res = state->ModuleManager->LoadModule("editor");
		RAY_ASSERT(res.IsSuccess(), "failed to load engine");

		EditorInterface* editor = (EditorInterface*)res.Data->QueryModuleInterface();
		_engine = editor->CreateEditorEngine();
		state->Engine = _engine;
	}
	else
	{
		gLauncherLog->Log("------- Beginning of RayEngine");

		/* 2. Load the engine module. This will register the objects we need. */
		auto res = state->ModuleManager->LoadModule("engine");
		RAY_ASSERT(res.IsSuccess(), "failed to load engine")

		/* 2. Create the Engine class. */
		_engine = new RayEngine();
		state->Engine = _engine;
	}
}

void EngineLoop::Initialize()
{
	_engine->Initialize(this);
}

void EngineLoop::Tick()
{
	_engine->Tick();
}

EngineLoop::~EngineLoop()
{
	IRayState* state = RayState();
	delete state->FileSystem;
	delete state->ModuleManager;
	delete state->ObjectDb;
	delete state->Debug;
	
	delete state;
	delete _engine;
}
