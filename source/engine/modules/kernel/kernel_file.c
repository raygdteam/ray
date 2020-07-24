#include "kernel.h"
#include <Windows.h>

typedef rayHandle rayFileHandle;

rayFileHandle krnlFileOpen(const char* name)
{
	rayFileHandle file = CreateFileA(name, GENERIC_READ, 0, 0, 0, 0, 0);

	if (file == INVALID_HANDLE_VALUE)
		return RAY_INVALID_HANDLE;

	return file;
}
