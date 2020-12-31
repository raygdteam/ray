#include "memory_manager.hpp"

//temporary
#include <Windows.h>

namespace ray
{
	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Initialize(size_t preferredSize) noexcept
	{
		_preferredPoolSize = preferredSize;

		_listOfPools.Buffer = static_cast<MemoryPool*>(VirtualAlloc(nullptr, KB(64), MEM_COMMIT, PAGE_READWRITE));
		_listOfPools.Buffer[_listOfPools.ElementsCount++].Create(preferredSize);
	}

	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Destroy() noexcept
	{
		if (_listOfPools.ElementsCount != -1)
		{
			for (size_t i = 0; i < _listOfPools.ElementsCount; ++i)
				_listOfPools.Buffer[i].Destroy();

			VirtualFree(_listOfPools.Buffer, 0, MEM_RELEASE);
			_listOfPools.ElementsCount = -1;
		}
	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::RequestPool(size_t requestedSize) noexcept
	{
		auto& pool = _listOfPools.Buffer[_listOfPools.ElementsCount - 1];

		if (pool.IsEnough(requestedSize))
			return pool;

		size_t size = requestedSize > _preferredSize ? requestedSize : _preferredSize;
		return CreatePool(size);
	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::CreatePool(size_t requestedSize) noexcept
	{
		_listOfPools.Buffer[_listOfPools.ElementsCount].Create(preferredSize);
		return _listOfPools.Buffer[_listOfPools.ElementsCount++];
	}
}