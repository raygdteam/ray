#include "pch.hpp"
#include "memory_manager.hpp"
#include "std_alloc.hpp"
#include "platform/platform_memory.hpp"

namespace ray::core::memory
{

IAllocator* MemoryManager::_allocator = nullptr;

void MemoryManager::Initialize()
{
	auto allocator = (StdAlloc*)platform::PlatformMemory::LowLevelAlloc(sizeof(StdAlloc));
	new (allocator) StdAlloc();
	_allocator = static_cast<IAllocator*>(allocator);
}

IAllocator* MemoryManager::GetAllocator()
{
	if (_allocator == nullptr) Initialize();
	return _allocator;
}

void MemoryManager::Shutdown()
{
	// FREE() IS IMPORTANT HERE!!!
	platform::PlatformMemory::LowLevelFree(_allocator);
}
}
