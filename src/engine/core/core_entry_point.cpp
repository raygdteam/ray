#include "pch.hpp"
#include "memory/memory.hpp"
#include "raytl/array.hpp"

extern "C" RAY_DLLIMPORT void dummy();

u32 __stdcall DllMain(void* instance, unsigned long reason, void*)
{
	dummy();
	return 1;
}

MEMORY_OVERRIDE;
