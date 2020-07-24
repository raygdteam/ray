#pragma once

#ifdef RAY_BUILD_KERNEL
#define RAY_KERNEL_API __declspec(dllexport)
#else
#define RAY_KERNEL_API __declspec(dllimport)
#endif

typedef void* rayHandle;
