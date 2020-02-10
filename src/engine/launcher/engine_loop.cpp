#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"
#include "core/file_system/file_system.hpp"
#include "core/logging/log.hpp"
#include "core/platform/platform_fs.hpp"

namespace ray::launcher
{

logging::ILog log(L"EngineLoop");

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	FileSystem::Get().initialize();

	log.log(L"Initializing: {}", true);

	core::platform::PlatformFS::OpenFile(L"common/ray/os/include.hpp");
}

void EngineLoop::Initialize()
{
}

void EngineLoop::Tick()
{

}
}
