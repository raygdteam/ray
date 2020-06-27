#include <Windows.h>


void krnlInstallHook(void* old, void* new)
{
    unsigned char patch[] = {
        0x48, 0xb8, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xE0
    };

    *(DWORD64*)&patch[2] = (DWORD64)new;

    DWORD oldprot;
    VirtualProtect(old, 1, PAGE_EXECUTE_READWRITE, &oldprot);
    memcpy(old, patch, sizeof(patch));
    VirtualProtect(old, 1, oldprot, &oldprot);

    FlushInstructionCache(GetCurrentProcess(), old, 10);
}