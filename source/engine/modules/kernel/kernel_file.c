#include <Windows.h>
#include <Shlwapi.h>
#include "kernel_file.h"

#pragma comment(lib, "shlwapi.lib")

void palFileSysSwitchToExeDirectory()
{
    char exePath[260];
    GetModuleFileNameA(NULL, exePath, 260);
    PathRemoveFileSpecA(exePath);
    SetCurrentDirectoryA(exePath);
}

rayFileHandle krnlFileOpen(const char* path, int mode)
{
    DWORD access, share;

    switch (mode)
    {
    case OF_READ:
        access = GENERIC_READ;
        break;
    case OF_WRITE:
        access = GENERIC_WRITE;
        break;
    case OF_READWRITE:
        access = GENERIC_READ | GENERIC_WRITE;
        break;
    default:
        access = 0;
        break;
    }

    share = mode & (OF_SHARE_COMPAT | OF_SHARE_DENY_NONE | OF_SHARE_DENY_READ | OF_SHARE_DENY_WRITE | OF_SHARE_EXCLUSIVE);

    switch (share)
    {
    case OF_SHARE_DENY_READ:
        share = FILE_SHARE_WRITE;
        break;
    case OF_SHARE_DENY_WRITE:
        share = FILE_SHARE_READ;
        break;
    case OF_SHARE_DENY_NONE:
        share = FILE_SHARE_READ | FILE_SHARE_WRITE;
        break;
    case OF_SHARE_EXCLUSIVE:
        share = 0;
        break;
    case OF_SHARE_COMPAT:
        share = FILE_SHARE_READ | FILE_SHARE_WRITE;
        break;
    default:
        share = 0;
        break;
    }

    rayFileHandle unnamed = CreateFileA(path, access, share, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (unnamed == INVALID_HANDLE_VALUE)
        return RAY_INVALID_HANDLE;

    return unnamed;
}

void krnlFileClose(rayFileHandle file)
{
    if (file == RAY_INVALID_HANDLE)
        return;

    if (CloseHandle(file) != FALSE)
    {
        /* todo: handle failure correctly */
        __debugbreak();
        return;
    }
}

unsigned long long krnlFileRead(rayFileHandle file, void* buffer, unsigned long numBytes)
{
    DWORD numRead = 0;

    if (!numBytes)
        numRead = SetEndOfFile(file) ? 0 : HFILE_ERROR;
    else
    {
        if (ReadFile(file, buffer, numBytes, &numRead, NULL) == FALSE)
        {
            __debugbreak();
            numRead = HFILE_ERROR;
        }
    }

    return numRead;
}

unsigned long long krnlFileWrite(rayFileHandle file, void* buffer, unsigned long numBytes)
{
    DWORD numWrite = 0;

    if (!numBytes)
        numWrite = SetEndOfFile(file) ? 0 : HFILE_ERROR;
    else
    {
        if (WriteFile(file, buffer, numBytes, &numWrite, NULL) == FALSE)
        {
            __debugbreak();
            numWrite = HFILE_ERROR;
        }
    }

    return numWrite;
}
