#pragma once
#include "allocator.hpp"

namespace ray::core::memory
{

class StdMalloc 
	: public IAllocator
{
public:
	~StdMalloc() override {}

	void* Alloc(size_t Size, size_t Alignment = 16) override;
	void Free(void*);
	void* Realloc(void*, size_t, size_t) override;
	size_t GetAllocationSize(void*) override;

};

}
