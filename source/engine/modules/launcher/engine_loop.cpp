#include "engine_loop.hpp"
#include "../core/core.hpp"


namespace ray::launcher
{

void EngineLoop::PreInitialize()
{
	// core::platform::PlatformFS::Initialize();

	_engine = new RayEngine();
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
