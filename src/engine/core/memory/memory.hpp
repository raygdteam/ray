#pragma once
#include "core/core.hpp"

namespace ray::core::memory
{

class RAY_CORE_API Memory
{
public:
	static void Memset(void* data, u8 value, size_t size);

	template<typename T>
	static void Memset(T& data, u8 value);

	static void Memcpy(void* from, void* to, size_t size);

	static void* Allocate(size_t size);
	static void* Reallocate(void* original, size_t size, u32 alignment);
	static void Free(void* data);
	static size_t GetAllocationSize(void* data);
};


#define MEMORY_OVERRIDE void* operator new(size_t size) { return ::ray::core::memory::Memory::Allocate(size); } \
	void* operator new[](size_t size) { return ::ray::core::memory::Memory::Allocate(size); } \
	void operator delete(void* data) { ::ray::core::memory::Memory::Free(data); } \
	void operator delete[](void* data) { ::ray::core::memory::Memory::Free(data); } \
	void operator delete(void* data, size_t size) { ::ray::core::memory::Memory::Free(data); } \
	void operator delete[](void* data, size_t size) { ::ray::core::memory::Memory::Free(data); } \
	int __cdecl _purecall() { return 0; }
	/* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	 * Workaround for this linker error:
	 *   error LNK2001: unresolved external symbol _purecall
	 * TODO: move this shit out of memory.hpp.
	 */

}