#include <Windows.h>
#include "kernel_win32_critical_section.h"

rayHandle krnlWin32CreateCriticalSection()
{
	CRITICAL_SECTION* cs = (CRITICAL_SECTION*)malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(cs);
	return cs;
}

void krnlWin32EnterCriticalSection(rayHandle cs)
{
	EnterCriticalSection(cs);
}

unsigned short krnlWin32TryEnterCriticalSection(rayHandle cs)
{
	return TryEnterCriticalSection(cs);
}

void krnlWin32LeaveCriticalSection(rayHandle cs)
{
	LeaveCriticalSection(cs);
}

void krnlWin32DeleteCriticalSection(rayHandle cs)
{
	DeleteCriticalSection(cs);
	free(cs);
}
