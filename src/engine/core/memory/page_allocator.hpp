#pragma once
#include "core/core.hpp"

namespace ray::core::memory
{
	const u16 PAGE_SIZE = static_cast<u16>(64 * 1024);

	class PageAllocator
	{
	public:
		void* Allocate();
		void Free(void*);

	private:

	};
}

