#include "pch.hpp"
#include "memory.hpp"
#include "memory_manager.hpp"
#include "platform/platform_memory.hpp"

using namespace ray::core::platform;

namespace ray::core::memory
{

void Memory::Memset(void* data, u8 value, size_t size)
{
	PlatformMemory::Memset(data, value, size);
}

template <typename T>
void Memory::Memset(T& data, u8 value)
{
	static_assert(std::is_pointer<T>::value, "For pointers use three-parameter memset()");
	Memset(&data, value, sizeof(T));
}

void Memory::Memcpy(void* from, void* to, size_t size)
{
	PlatformMemory::Memcpy(from, to, size);
}

void* Memory::Allocate(size_t size)
{
	return MemoryManager::GetAllocator()->Alloc(size, 0);
}

void* Memory::Reallocate(void* original, size_t size, u32 alignment)
{
	return MemoryManager::GetAllocator()->Realloc(original, size, alignment);
}

void Memory::Free(void* data)
{
	MemoryManager::GetAllocator()->Free(data);
}

size_t Memory::GetAllocationSize(void* data)
{
	return MemoryManager::GetAllocator()->GetAllocationSize(data);
}
}
