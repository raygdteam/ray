#include "pch.hpp"
#include "StdMalloc.hpp"
#include "core.h"

namespace ray::core::memory
{

	void* StdMalloc::Alloc(size_t Size, size_t Alignment)
	{
		RAY_ASSERT((Alignment & (Alignment >> 1)) == 0, "Alignement must be power of 2!");
#ifdef RAY_COMPILER_MSVC
		return _aligned_malloc(Size, Alignment);
#elif defined (RAY_COMPILER_GCC)
		return aligned_alloc(Alignment, Size);
#endif 
	}

	void StdMalloc::Free(void* MemBlock)
	{
		free(MemBlock);
	}

	void* StdMalloc::Realloc(void* MemBlock, size_t Size)
	{
		return realloc(MemBlock, Size);
	}

}

