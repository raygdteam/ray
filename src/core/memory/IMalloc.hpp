#pragma once

namespace ray::core::memory
{
	
	class IMalloc
	{
	public:
		virtual ~IMalloc();

		virtual void* Alloc(size_t, size_t) = 0;
		virtual void Free(void*) = 0;
		virtual void* Realloc(void*, size_t, size_t) = 0;
		virtual inline size_t GetAllocationSize(void*) = 0;

	};

}


