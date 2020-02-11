#pragma once
#include "core/core.hpp"
#include "allocator.hpp"

namespace ray::core::memory
{

class RAY_CORE_API MemoryManager
{
	static IAllocator* _allocator;
public:
	static void Initialize();
	static IAllocator* GetAllocator();
	static void Shutdown();
};

}