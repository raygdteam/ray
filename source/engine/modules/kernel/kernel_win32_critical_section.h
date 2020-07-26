#include "kernel.h"

/** Creates critical section object */
RAY_KERNEL_API rayHandle krnlWin32CreateCriticalSection();

/** Blocking enter critical section. */
RAY_KERNEL_API void krnlWin32EnterCriticalSection(rayHandle cs);

/** Non-blocking attempt to enter critical section. returns 0 on failure, 1 on success. */
RAY_KERNEL_API unsigned short krnlWin32TryEnterCriticalSection(rayHandle cs);

/** Leaves critical section. */
RAY_KERNEL_API void krnlWin32LeaveCriticalSection(rayHandle cs);

/** Deletes critical section object. */
RAY_KERNEL_API void krnlWin32DeleteCriticalSection(rayHandle cs);
