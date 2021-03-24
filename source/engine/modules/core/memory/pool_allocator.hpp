#pragma once
#include <eastl/map.h>
#include <core/core.hpp>

#define POOL_ALLOCATOR_ERROR { ~0u }

/// <summary>
/// Used by memory pool and manages its memory
/// </summary>
class RAY_CORE_API PoolAllocator
{
protected:
	struct MemoryFragment;
	/// <summary>
	/// Type of the map that keeps memory blocks sorted by their offsets
	/// </summary>
	using MemoryFragmentsByOffsetMap = eastl::map<size_t, MemoryFragment>;

	/// <summary>
	/// Type of the map that keeps memory blocks sorted by their sizes
	/// </summary>
	using MemoryFragmentsBySizeMap = eastl::multimap<size_t, MemoryFragmentsByOffsetMap::iterator>;

	/// <summary>
	/// Describes memory block that not reserved
	/// </summary>
	struct MemoryFragment
	{
		MemoryFragment(size_t _Size) : Size(_Size) {}

		size_t Size;
		MemoryFragmentsBySizeMap::iterator OrderBySizeIt;
	};

	MemoryFragmentsByOffsetMap _fragmentsByOffset;
	MemoryFragmentsBySizeMap _fragmentsBySize;

	size_t _maxAvailableSpace;
	size_t _poolSize;

public:
	PoolAllocator() noexcept = default;

	explicit PoolAllocator(size_t poolSize) noexcept
		: _maxAvailableSpace(poolSize)
		, _poolSize(poolSize)
	{
		AddNewFragment(0, poolSize);
	}

	/*PoolAllocator(PoolAllocator&& rhs) = default;
	PoolAllocator& operator = (PoolAllocator&& rhs) = default;

	PoolAllocator(const PoolAllocator& lhs) = default;
	PoolAllocator& operator = (const PoolAllocator& lhs) = default;*/

public:
	void SetPoolSize(size_t poolSize) noexcept
	{
		_poolSize = poolSize;
	}

	size_t GetPoolSize() const noexcept
	{
		return _poolSize;
	}

	size_t GetMaxAvailableSpace() const noexcept
	{
		return _maxAvailableSpace;
	}

public:
	/// <summary>
	/// Reserves space within pool
	/// </summary>
	/// <param name="size">- Required size</param>
	/// <returns>Offset relative a begining of pool</returns>
	virtual size_t Allocate(size_t size) noexcept;

	/// <summary>
	/// Releases space within pool
	/// </summary>
	/// <param name="offset">- Offset relative a begining of pool</param>
	/// <param name="size">- Size that we need to release</param>
	void Free(size_t offset, size_t size) noexcept;

protected:
	void AddNewFragment(size_t offset, size_t size) noexcept;

};