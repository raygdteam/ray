#pragma once

namespace ray::core::memory
{
	
	class IMalloc
	{
	public:
		virtual ~IMalloc();

		virtual void* Alloc(size_t Size, size_t Align = 0) = 0;
		virtual void Free(void*) = 0;
		virtual void* Realloc(void*, size_t) = 0;

	};

}


