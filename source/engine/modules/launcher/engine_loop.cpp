#include "engine_loop.hpp"
#include <core/core.hpp>
#include <engine/state/state.hpp>


namespace ray::launcher
{
	
void EngineLoop::PreInitialize()
{
	/* 1. Initialize RayState and vital core components. */
	/* First call to RayState() will allocate the memory. */
	IRayState* state = RayState();
	state->ObjectDb = new ObjectDb();
	state->ModuleManager = new ModuleManager();

	/* 2. Load the engine module. This will register the objects we need. */
	auto res = state->ModuleManager->LoadModule("engine");
	RAY_ASSERT(res.IsSuccess(), "failed to load engine")

	/* 2. Create the Engine class. */
	_engine = new RayEngine();
	state->Engine = _engine;
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
	delete _engine;
}
	
}
