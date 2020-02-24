#pragma once
#include "core/core.hpp"

namespace ray::core::platform
{

enum FileOpenMode
{
	eRead = 0x8L,
	eWrite = 0x4L,

	eReadWrite = eRead | eWrite
};

struct PlatformFileHandle
{
	FileOpenMode AccessMode;
	void* FileHandle;
	void* MappingHandle;
	size_t Size;
	void* Buffer;
};

class RAY_CORE_API PlatformFS
{
public:
	static void Initialize();
	static PlatformFileHandle OpenFile(pcstr path, FileOpenMode mode = eReadWrite);
	static void CloseFile(void* file);

	static void WriteFile(pcstr path, size_t size, void* data);
};
}
