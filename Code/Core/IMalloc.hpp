#pragma once

namespace ray::core::memory
{
	
	class IMalloc
	{
	public:
		IMalloc() : m_AllocatedMemorySize(0) {}
		virtual ~IMalloc();

		virtual void* Alloc(size_t, size_t) = 0;
		virtual void Free(void*) = 0;
		virtual void* Realloc(void*, size_t) = 0;
		inline size_t GetAllocatedMemorySize() { return m_AllocatedMemorySize; }

	protected:
		size_t m_AllocatedMemorySize;

	};

}


