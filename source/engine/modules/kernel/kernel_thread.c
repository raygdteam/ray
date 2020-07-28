#define _CRT_SECURE_NO_WARNINGS
#include "kernel_thread.h"
#include <windows.h>

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

void krnlJoinThread(rayHandle thread)
{
	WaitForSingleObject(thread, INFINITE);
}


#pragma pack(push, 8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void krnlSetThreadName(const char* name)
{
	size_t cSize = strlen(name) + 1;
	wchar_t* wideName = malloc(sizeof(wchar_t) * cSize);
	mbstowcs(wideName, name, cSize);
	
	SetThreadDescription(GetCurrentThread(), wideName);

	free(wideName);

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name;
	info.dwThreadID = GetCurrentThreadId();
	info.dwFlags = 0;
	
	__try 
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{}
}
