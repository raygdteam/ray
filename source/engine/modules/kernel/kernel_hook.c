#include <Windows.h>

void _memcpy(void* to, void* from, size_t size)
{
    char* csrc = (char*)to;
    char* cdest = (char*)from;

    // Copy contents of src[] to dest[] 
    for (int i = 0; i < size; i++)
        cdest[i] = csrc[i];
}

void krnlInstallHook(void* old, void* new)
{
    unsigned char patch[] = {
        0x48, 0xb8, 0x00,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xE0
    };

    *(DWORD64*)&patch[2] = (DWORD64)new;

    DWORD oldprot;
    VirtualProtect(old, 1, PAGE_EXECUTE_READWRITE, &oldprot);
    _memcpy(old, patch, sizeof(patch));
    VirtualProtect(old, 1, oldprot, &oldprot);

    FlushInstructionCache(GetCurrentProcess(), old, 10);
}