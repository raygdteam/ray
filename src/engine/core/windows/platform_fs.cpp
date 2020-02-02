#include "pch.hpp"
#include "platform/platform_fs.hpp"
#include <ray/os/include.hpp>

namespace ray::core::platform
{

void PlatformFS::Initialize()
{
	wchar_t* path = new wchar_t[500];

	GetCurrentDirectory(500, path);
	lstrcat(path, L"\\..\\..");

	SetCurrentDirectoryW(path);

	GetCurrentDirectory(500, path);

	delete[] path;
}

}
