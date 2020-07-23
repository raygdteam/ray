#pragma once
#include "kernel.h"

typedef void* rayHandle;

typedef unsigned long long(__stdcall* THREAD_START_PROC)(void*);

RAY_KERNEL_API rayHandle krnlCreateThread(unsigned stackSize, THREAD_START_PROC threadStartProc, void* parameter, unsigned long* outThreadId);
RAY_KERNEL_API void krnlResumeThread(rayHandle thread);
RAY_KERNEL_API unsigned long krnlGetThreadCount();
RAY_KERNEL_API void krnlJoinThread(rayHandle);
RAY_KERNEL_API void krnlSetThreadName(const char*);
