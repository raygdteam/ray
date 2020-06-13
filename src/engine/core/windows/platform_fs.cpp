#include "pch.hpp"
#include "platform/platform_fs.hpp"

#include <regex>
#include <ray/os/include.hpp>

namespace ray::core::platform
{

void PlatformFS::Initialize()
{
	char* path = new char[500];

	GetCurrentDirectory(500, path);
	lstrcat(path, "\\..\\..");

	SetCurrentDirectoryA(path);

    GetCurrentDirectory(500, path);

	delete[] path;
}

PlatformFileHandle PlatformFS::OpenFile(pcstr path, FileOpenMode mode)
{
	/* replace all "/"'s in the path with "\\" for Windows platform. */
	string fixedPath(path);
	fixedPath = std::regex_replace(fixedPath, std::regex("/"), "\\");
	
    HANDLE hFile = CreateFile(fixedPath.c_str(), mode, 0,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        return {};
    }

    DWORD dwFileSize = GetFileSize(hFile, nullptr);
    if (dwFileSize == INVALID_FILE_SIZE) 
    {
        CloseHandle(hFile);
        return {};
    }

    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
        0, 0, nullptr);
    if (hMapping == nullptr) 
    { // yes, NULL, not INVALID_HANDLE_VALUE, see MSDN
        CloseHandle(hFile);
        return {};
    }

    unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ,
        0, 0, dwFileSize);
    if (dataPtr == nullptr) 
    {
    	CloseHandle(hMapping);
        CloseHandle(hFile);
        return {};
    }

    return { mode, hFile, hMapping, dwFileSize, dataPtr };
}

void PlatformFS::CloseFile(void* file)
{
    CloseHandle(file);
}

void PlatformFS::WriteFile(pcstr path, size_t size, void* data)
{
    HANDLE hFile = CreateFile(path, GENERIC_WRITE, 0,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (!hFile)
        return;

	LPDWORD numWritten = 0;

    ::WriteFile(hFile, data, DWORD(size), numWritten, nullptr);
}
}
