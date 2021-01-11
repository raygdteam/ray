#include "core.hpp"
#include <core/memory/new_delete_override.hpp>
#include <core/lib/delegate.hpp>

extern "C" RAY_DLLIMPORT void dummy();

u32 __stdcall DllMain(void*, unsigned long, void*)
{
	dummy();
	return 1;
}

extern "C" RAY_DLLEXPORTS void _() {}
