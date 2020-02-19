#include "pch.hpp"
#include "platform/platform_fs.hpp"

#include <regex>
#include <ray/os/include.hpp>

namespace ray::core::platform
{

static logging::ILog log("WindowsPlatformFS");

void PlatformFS::Initialize()
{
	char* path = new char[500];

	GetCurrentDirectory(500, path);
	lstrcat(path, "\\..\\..");

	SetCurrentDirectoryA(path);

    GetCurrentDirectory(500, path);
	log.log("set working directory: {}", path);

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
        log.log("fileMappingCreate - CreateFile failed");
        return {};
    }

    DWORD dwFileSize = GetFileSize(hFile, nullptr);
    if (dwFileSize == INVALID_FILE_SIZE) 
    {
        log.log("fileMappingCreate - GetFileSize failed");
        CloseHandle(hFile);
        return {};
    }

    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
        0, 0, nullptr);
    if (hMapping == nullptr) 
    { // yes, NULL, not INVALID_HANDLE_VALUE, see MSDN
        log.log("fileMappingCreate - CreateFileMapping failed");
        CloseHandle(hFile);
        return {};
    }

    unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping, FILE_MAP_READ,
        0, 0, dwFileSize);
    if (dataPtr == nullptr) 
    {
        log.log("fileMappingCreate - MapViewOfFile failed");
    	CloseHandle(hMapping);
        CloseHandle(hFile);
        return {};
    }

    return { mode, hFile, hMapping, dwFileSize, dataPtr };
}
}
