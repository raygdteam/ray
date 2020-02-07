#include "engine_loop.hpp"
#include "core/memory/memory_manager.hpp"
#include "core/file_system/file_system.hpp"

#if defined(RAY_BUILD_RELEASE) && defined(RAY_BUILDING_EDITOR)
#error Editor in release - why?
#endif


namespace ray::launcher
{

void EngineLoop::PreInitialize(pcwstr commandLine, s32 argc)
{
	core::memory::MemoryManager::Initialize();
	FileSystem::Get().initialize();

}

void EngineLoop::Initialize()
{
}

void EngineLoop::Tick()
{

}
}
