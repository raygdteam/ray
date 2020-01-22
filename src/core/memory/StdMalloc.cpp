#include "pch.hpp"
#include "StdMalloc.hpp"
#include "core.h"
#include <algorithm>

namespace ray::core::memory
{

void* StdMalloc::Alloc(size_t Size, size_t Alignment)
{
	RAY_ASSERT((Alignment & (Alignment >> 1) == 0), "Alignment must be power of 2!")
	Alignment = std::max<u32>({ Size >= 16 ? u32(16) : u32(8) , u32(Alignment) });
	void* Result = nullptr;
#ifdef RAY_COMPILER_MSVC
	Result = _aligned_malloc(Size, Alignment);
#elif defined (RAY_COMPILER_GCC)
	posix_memalign(&Result, Alignment, NewSize);
#endif 
	return Result;
}

void StdMalloc::Free(void* MemBlock)
{
#ifdef RAY_COMPILER_MSVC
	_aligned_free(MemBlock);
#elif defined(RAY_COMPILER_GCC)
	free(MemBlock);
#endif
}

void* StdMalloc::Realloc(void* MemBlock, size_t NewSize, size_t Alignment)
{
	RAY_ASSERT((Alignment & (Alignment >> 1) == 0), "Alignment must be power of 2!")
	void* Result = nullptr;
	Alignment = std::max<u32>({ NewSize >= 16 ? u32(16) : u32(8) , u32(Alignment) });
#ifdef RAY_COMPILER_MSVC
	if (MemBlock && NewSize)
	{
		Result = _aligned_realloc(MemBlock, NewSize, Alignment);
	}
	else if (!MemBlock)
	{
		Result = _aligned_malloc(NewSize, Alignment);
	}
	else
	{
		_aligned_free(MemBlock);
	}
#elif defined(RAY_COMPILER_GCC)

	if (MemBlock && NewSize)
	{
		size_t UsableSize = malloc_usable_size(MemBlock);
		if (posix_memalign(&Result, Alignment, NewSize) == 0)
		{
			if(UsableSize)
				memcpy(Result, MemBlock, std::min<u32>({ NewSize, UsableSize }));
		}
		free(MemBlock);
	}
	else if (!MemBlock)
	{
		posix_memalign(&Result, Alignment, NewSize);
	}
	else
	{
		free(MemBlock);
	}

#endif
	return Result;
}

size_t StdMalloc::GetAllocationSize(void* MemBlock)
{
#ifdef RAY_COMPILER_MSVC
	return _aligned_msize(MemBlock, 16, 0);
#elif defined(RAY_COMPILER_GCC)
	return malloc_usable_size(MemBlock);
#endif
}

}

