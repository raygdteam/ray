#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"
#include "core/file_system/file_system.hpp"
#include "core/logging/log.hpp"
#include "core/modules/startup_initializer.hpp"

#include <ray/os/include.hpp>
#include <winternl.h>

namespace ray::launcher
{

logging::ILog log("EngineLoop");

extern "C" void* Get_Ldr_Addr();

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	/* Super early init code. DO NOT MOVE THIS ANYWHERE ELSE! */
	core::modules::InitializationManager::RunPhase(core::modules::InitializationPhase::eStartupPlugin);

	FileSystem::Get().initialize();
	log.log("Ray v{}.{}.{} {}", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);
	core::modules::InitializationManager::RunPhase(core::modules::InitializationPhase::ePreInitialization);
}

void EngineLoop::Initialize()
{
	
}

void EngineLoop::Tick()
{}

}
