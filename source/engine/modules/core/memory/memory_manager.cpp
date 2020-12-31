#include "memory_manager.hpp"

namespace ray
{
	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Initialize() noexcept
	{
		_listOfPools.Buffer = static_cast<MemoryPool*>(VirtualAlloc(nullptr, KB(64), MEM_COMMIT, PAGE_READWRITE));
		
	}

	template<typename MemoryPool>
	void MemoryManager<MemoryPool>::Destroy() noexcept
	{

	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::RequestPool(size_t requestedSize) noexcept
	{

	}

	template<typename MemoryPool>
	MemoryPool& MemoryManager<MemoryPool>::CreatePool(size_t requestedSize) noexcept
	{

	}
}