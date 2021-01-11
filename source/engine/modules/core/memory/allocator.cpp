#include "allocator.hpp"

	static MemoryAllocatorManager sMallocManager;

	MemoryPool* MemoryAllocatorManager::RequesPool(size_t requestedSize)
	{
		return nullptr;
	}

	MemoryPool* MemoryAllocatorManager::CreateNewPool()
	{
		return nullptr;
	}

	void* MemoryAllocator::Allocate(size_t size)
	{
		return nullptr;
	}

	void MemoryAllocator::Free(void* ptr)
	{

	}