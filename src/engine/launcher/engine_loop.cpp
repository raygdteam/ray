#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"

#include <ray/os/include.hpp>
#include <winternl.h>
#include "core/platform/platform_fs.hpp"

namespace ray::launcher
{

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	core::platform::PlatformFS::Initialize();

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
