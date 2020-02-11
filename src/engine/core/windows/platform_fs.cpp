#include "pch.hpp"
#include "platform/platform_fs.hpp"

#include <regex>
#include <ray/os/include.hpp>

namespace ray::core::platform
{

static logging::ILog log(L"WindowsPlatformFS");

void PlatformFS::Initialize()
{
	wchar_t* path = new wchar_t[500];

	GetCurrentDirectory(500, path);
	lstrcat(path, L"\\..\\..");

	SetCurrentDirectoryW(path);
	log.log(L"set working directory: {}", path);

	delete[] path;
}

PlatformFileHandle PlatformFS::OpenFile(pcwstr path, FileOpenMode mode)
{
	/* replace all "/"'s in the path with "\\" for Windows platform. */
	string fixedPath(path);
	fixedPath = std::regex_replace(fixedPath, std::wregex(L"/"), L"\\");
	
    HANDLE hFile = CreateFile(fixedPath.c_str(), mode, 0,
        nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        log.log(L"fileMappingCreate - CreateFile failed");
        return {};
    }

    DWORD dwFileSize = GetFileSize(hFile, nullptr);
    if (dwFileSize == INVALID_FILE_SIZE) 
    {
        log.log(L"fileMappingCreate - GetFileSize failed");
        CloseHandle(hFile);
        return {};
    }

    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
        0, 0, nullptr);
    if (hMapping == nullptr) 
    { // yes, NULL, not INVALID_HANDLE_VALUE, see MSDN
        log.log(L"fileMappingCreate - CreateFileMapping failed");
        CloseHandle(hFile);
        return {};
    }

    unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ,
        0, 0, dwFileSize);
    if (dataPtr == nullptr) 
    {
        log.log(L"fileMappingCreate - MapViewOfFile failed");
    	CloseHandle(hMapping);
        CloseHandle(hFile);
        return {};
    }

    return { mode, hFile, hMapping, dwFileSize, dataPtr };
}
}
