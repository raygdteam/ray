#include "pch.hpp"
#include "page_allocator.hpp"
#include <ray/os/include.hpp>

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