#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"
#include "core/file_system/file_system.hpp"
#include "core/logging/log.hpp"
#include "core/platform/platform_fs.hpp"

#include <ray/os/include.hpp>
#include <winternl.h>

namespace ray::launcher
{

logging::ILog log(L"EngineLoop");

extern "C" void* Get_Ldr_Addr();

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	FileSystem::Get().initialize();
	log.log(L"Ray {}.{}.{} {}", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);

	// PEB* peb = (PEB*)Get_Ldr_Addr();
	
}

void EngineLoop::Initialize()
{}

void EngineLoop::Tick()
{

}
}
