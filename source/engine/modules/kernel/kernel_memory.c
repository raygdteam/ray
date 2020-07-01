#include <Windows.h>
#include <crtdbg.h>
#define MI_SHARED_LIB
#include "../../thirdparty/mimalloc/mimalloc.h"

void krnlInstallHook(void* old, void* new);


void krnlInitMemory()
{
	krnlInstallHook(&malloc, &mi_malloc);
	krnlInstallHook(&free, &mi_free);

	krnlInstallHook(&_aligned_malloc, &mi_malloc_aligned);
	krnlInstallHook(&_aligned_free, &mi_free_aligned);
}