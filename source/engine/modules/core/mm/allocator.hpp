#pragma once
#include <core/core.hpp>

struct IAllocator
{
	/**
	 * Initializes the allocator.
	 */
	virtual bool Initialize() = 0;

	/**
	 * Allocate the memory block.
	 */
	virtual void* Allocate(u64 size, u8 alignment = 2) = 0;

	/**
	 * Free the memory block.
	 */
	virtual void Free(void* block) = 0;

	/**
	 * Get the allocation size for given block.
	 */
	virtual u64 AllocationSize(void* block) = 0;
};
