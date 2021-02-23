#pragma once
#include <core/core.hpp>
#include <new>

// Default allocator which uses malloc and free from standart library
class DefaultAllocator
{
public:
	NODISCARD void* Allocate(size_t sizeInBytes) noexcept
	{
		return malloc(sizeInBytes);
	}

	NODISCARD void* Reallocate(void* mem, size_t newSize) noexcept
	{
		return realloc(mem, newSize);
	}

	void Free(void* mem) noexcept
	{
		free(mem);
	}

};