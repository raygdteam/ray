#include "core.hpp"

extern "C" RAY_DLLIMPORT void dummy();

u32 __stdcall DllMain(void* instance, unsigned long reason, void*)
{
	dummy();
	return 1;
}

extern "C" RAY_DLLEXPORTS void _() {}
