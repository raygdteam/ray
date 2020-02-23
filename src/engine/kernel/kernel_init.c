#include <windows.h>

static int _process_init(void) 
{
    /*void* mem = &HeapAlloc;

    DWORD old = 0;
    auto x = VirtualProtect(mem, 4, PAGE_EXECUTE_READWRITE, &old);
    auto y = GetLastError();

    unsigned char* a = (unsigned char*)mem;

    *a = 0; a++;
    *a = 0; a++;
    *a = 0; a++;*/

	return 0;
}


#ifdef RAY_BUILD_RELEASE
// MSVC: use data section magic for static libraries
// See <https://www.codeguru.com/cpp/misc/misc/applicationcontrol/article.php/c6945/Running-Code-Before-and-After-Main.htm>
typedef int(*_crt_cb)(void);

__pragma(comment(linker, "/include:" "_msvc_initu"))
#pragma section(".CRT$XIU", long, read)

#pragma data_seg(".CRT$XIU")
_crt_cb _msvc_initu[] = { &_process_init };
#pragma data_seg()

#else

//BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID reserved) 
BOOL WINAPI _DllMainCRTStartup(
    HINSTANCE const instance,
    DWORD     const reason,
    LPVOID    const reserved
)
{
    if (reason == DLL_PROCESS_ATTACH)
        _process_init();
    
    return TRUE;
}

// dummy exported function to enable .lib generation
__declspec(dllexport) void dummy()
{
    (void*)0x0;
}

#endif
