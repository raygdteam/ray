#include <windows.h>
#include "../core/memory/memory_debug.hpp"

void krnlInitMemory();

static int _process_init(void) 
{
#ifndef DEBUG_MEMORY
    krnlInitMemory();
#endif
	return 0;
}


#ifdef _RELEASE
// MSVC: use data section magic for static libraries
// See <https://www.codeguru.com/cpp/misc/misc/applicationcontrol/article.php/c6945/Running-Code-Before-and-After-Main.htm>
typedef int(*_crt_cb)(void);

__pragma(comment(linker, "/include:" "_msvc_initu"))
#pragma section(".CRT$XIU", long, read)

#pragma data_seg(".CRT$XIU")
_crt_cb _msvc_initu[] = { &_process_init };
#pragma data_seg()

#else

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved) 
/*BOOL WINAPI _DllMainCRTStartup(
    HINSTANCE const instance,
    DWORD     const reason,
    LPVOID    const reserved
)*/
{
    if (reason == DLL_PROCESS_ATTACH)
        _process_init();
    
    return TRUE;
}

// dummy exported function to enable .lib generation
__declspec(dllexport) void dummy()
{
    asm("nop");
}

#endif
