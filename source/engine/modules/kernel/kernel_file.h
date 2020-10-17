#pragma once

#include "kernel.h"

typedef rayHandle rayFileHandle;

RAY_KERNEL_API void palFileSysSwitchToExeDirectory();

RAY_KERNEL_API rayFileHandle krnlFileOpen(const char* path, int mode);
