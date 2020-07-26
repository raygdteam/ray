#include <Windows.h>
#include "kernel.h"

typedef rayHandle rayFileHandle;

rayFileHandle krnlFileOpen(const char* name)
{
	rayFileHandle file = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (file == INVALID_HANDLE_VALUE)
		return RAY_INVALID_HANDLE;

	return file;
}

void krnlFileClose(rayFileHandle file)
{
	if (file == RAY_INVALID_HANDLE)
	{
		return;
	}

	if (CloseHandle(file) != 0)
	{
		/* todo: handle failure correctly */
		__debugbreak();
		return;
	}
}

unsigned long long krnlFileRead(rayFileHandle file, void* buffer, unsigned long numBytes)
{
	DWORD numRead = 0;

	/* returns FALSE on error */
	if (ReadFile(file, buffer, numBytes, &numRead, NULL) == FALSE)
	{
		__debugbreak();
		return ((unsigned long long)-1);
	}

	return numRead;
}
