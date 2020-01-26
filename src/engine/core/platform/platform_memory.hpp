#pragma once
#include "core.hpp"

namespace ray::core::platform
{

class RAY_CORE_API PlatformMemory
{
public:
	// Low level memory management
	static void* LowLevelAlloc(size_t size);
	static void LowLevelFree(void* data);
	static void Memcpy(void* from, void* to, size_t size);
	static void Memset(void* data, u8 value, size_t size);

	// TODO: implement functions for getting OS memory stats etc.
};

}