#include "pch.hpp"
#include "StdMalloc.hpp"
#include "core.hpp"
#include "platform/platform_memory.hpp"
#include "memory.hpp"
#include <memory>

using namespace ray::core::platform;

namespace ray::core::memory
{

void* aligned_malloc(size_t size, size_t alignment)
{
	void* data = PlatformMemory::LowLevelAlloc(size + alignment + sizeof(void*) + sizeof(size_t));
	void* result = nullptr;

	if (data != nullptr)
	{
		// Слава всем богам если они есть если что тут нет ошибок
		/*result = reinterpret_cast<void*>(static_cast<u64>(reinterpret_cast<u8>(data) + sizeof(void*) + sizeof(size_t)) +
			alignment - 1 & ~(alignment - 1));
		*(void**)((u8*)result - sizeof(void*)) = data;
		*((size_t*)((u8*)result - sizeof(void*) - sizeof(size_t))) = size;*/
		result = std::align(alignment, size, data, size);
	}

	return result;
}

void* StdMalloc::Alloc(size_t Size, size_t Alignment)
{
	RAY_ASSERT((((Alignment) & (Alignment >> 1)) == 0), "Alignment must be power of 2!")
	void* Result = nullptr;
	Result = aligned_malloc(Size, Alignment);
	return Result;
}

void StdMalloc::Free(void* MemBlock)
{
	PlatformMemory::LowLevelFree(MemBlock);
}

void* StdMalloc::Realloc(void* MemBlock, size_t NewSize, size_t Alignment)
{
	RAY_ASSERT((((Alignment) & (Alignment >> 1)) == 0), "Alignment must be power of 2!")
	void* Result = nullptr;

	if (MemBlock && NewSize)
	{
		// Reallocating might break the alignment
		Result = aligned_malloc(NewSize, Alignment);
		size_t size = GetAllocationSize(MemBlock);
		Memory::Memcpy(MemBlock, Result, (NewSize >= size ? size : NewSize));
		Free(MemBlock);
	}
	else if (!MemBlock)
	{
		Result = aligned_malloc(NewSize, Alignment);
	}
	else
	{
		Free(MemBlock);
	}
	return Result;
}

size_t StdMalloc::GetAllocationSize(void* MemBlock)
{
	return *(size_t*)((u8*)MemBlock - sizeof(void*) - sizeof(size_t));
}

}

