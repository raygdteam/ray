#pragma once

#include "kernel.h"

typedef rayHandle rayFileHandle;

RAY_KERNEL_API void palFileSysSwitchToExeDirectory();

RAY_KERNEL_API rayFileHandle krnlFileOpen(const char* path, int mode);
RAY_KERNEL_API void krnlFileClose(rayFileHandle file);

RAY_KERNEL_API unsigned long long krnlFileRead(rayFileHandle file, void* buffer, unsigned long numBytes);
