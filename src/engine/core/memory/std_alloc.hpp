#pragma once

#include "allocator.hpp"

namespace ray::core::memory
{
class StdAlloc : public IAllocator
{
public:
	void* Alloc(size_t size, size_t alignment) override;
	void Free(void* memory) override;
	void* Realloc(void* memory, size_t size, size_t alignment) override;
	size_t GetAllocationSize(void*) override;
};
}

