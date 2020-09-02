#pragma once
#include <core/core.hpp>

/**
 * Primary memory manager API.
 */
class RAY_CORE_API Memory
{
public:
	/**
	 * Initializes the memory manager.
	 * Crashes the engine on failure.
	 */
	static void Initialize();

	/**
	 * Allocate the memory block.
	 */
	static void* Allocate(u64 size, u8 alignment = 2);

	/**
	 * Free the memory block.
	 */
	static void Free(void* block);

	/**
	 * Get the allocation size for given block.
	 */
	static u64 AllocationSize(void* block);
};
