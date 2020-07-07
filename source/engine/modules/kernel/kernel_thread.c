#include "kernel_thread.h"
#include <windows.h>

#define RAY_INVALID_HANDLE ((rayHandle)-1)

volatile long gThreadCount = 0;

struct RayThreadStartParams
{
	void* ThreadStartProc;
	void* Parameter;
};

DWORD __stdcall RayThreadEntry(void* param)
{
	InterlockedIncrement(&gThreadCount);

	struct RayThreadStartParams* params = (struct RayThreadStartParams*)param;
	((THREAD_START_PROC)params->ThreadStartProc)(params->Parameter);

	InterlockedDecrement(&gThreadCount);
	free(params);
	return 0;
}

rayHandle krnlCreateThread(unsigned stackSize, THREAD_START_PROC threadStartProc, void* parameter, unsigned long* outThreadId)
{
	if (threadStartProc == NULL) return RAY_INVALID_HANDLE;

	unsigned long long threadId = 0;

	struct RayThreadStartParams* params = malloc(sizeof(struct RayThreadStartParams));

	params->ThreadStartProc = threadStartProc;
	params->Parameter = parameter;

	HANDLE result = CreateThread(0, stackSize, RayThreadEntry, params, CREATE_SUSPENDED, outThreadId);

	if (result == NULL)
	{
		if (outThreadId != NULL) *outThreadId = 0;
		return RAY_INVALID_HANDLE;
	}

	return result;
}

void krnlResumeThread(rayHandle thread)
{
	if (thread != NULL)
		ResumeThread(thread);
}

unsigned long krnlGetThreadCount()
{
	return gThreadCount;
}
