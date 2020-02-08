#include "page_allocator.hpp"
#include "../core.hpp"
#include <ray/type/extensions.hpp>

namespace ray::core::memory
{
	//Windows only
	void* PageAllocator::Allocate()
	{
		return VirtualAlloc(0, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	}

	void PageAllocator::Free(void* Mem)
	{
		//TODO
	}

}