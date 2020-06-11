#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"
#include "core/file_system/file_system.hpp"
#include "core/modules/startup_initializer.hpp"

#include <ray/os/include.hpp>
#include <winternl.h>
#include "core/platform/platform_fs.hpp"

namespace ray::launcher
{

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	/* Super early init code. DO NOT MOVE THIS ANYWHERE ELSE! */
	core::modules::InitializationManager::RunPhase(core::modules::InitializationPhase::eStartupPlugin);

	core::platform::PlatformFS::Initialize();
	core::modules::InitializationManager::RunPhase(core::modules::InitializationPhase::ePreInitialization);

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
