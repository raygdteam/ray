#include <core/mm/memory_manager.hpp>

void Memory::Initialize()
{
	*(u16*)nullptr = 0xDED;
}

void* Memory::Allocate(u64 size, u8 alignment)
{
	return nullptr;
}

void Memory::Free(void* block)
{
	
}

u64 Memory::AllocationSize(void* block)
{
	return 0;
}
