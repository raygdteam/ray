#pragma once
#include "core/core.hpp"

namespace ray::core::memory
{
	const u16 PAGE_SIZE = 64 * 1024;

	class PageAllocator
	{
	public:
		void* Allocate();
		void Free(void*);

	private:

	};
}

