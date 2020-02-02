#include "pch.hpp"
#include "memory_manager.hpp"
#include "StdMalloc.hpp"
#include "platform/platform_memory.hpp"

namespace ray::core::memory
{

IAllocator* MemoryManager::_allocator = nullptr;

void MemoryManager::Initialize()
{
	auto allocator = (StdMalloc*)platform::PlatformMemory::LowLevelAlloc(sizeof(StdMalloc));
	new (allocator) StdMalloc();
	_allocator = static_cast<IAllocator*>(allocator);
}

IAllocator* MemoryManager::GetAllocator()
{
	return _allocator;
}

void MemoryManager::Shutdown()
{
	// FREE() IS IMPORTANT HERE!!!
	platform::PlatformMemory::LowLevelFree(_allocator);
}
}
