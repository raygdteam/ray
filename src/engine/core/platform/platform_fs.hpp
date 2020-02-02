#pragma once
#include "core/core.hpp"

namespace ray::core::platform
{

enum FileOpenMode
{
	eRead,
	eWrite,

	eReadWrite = eRead | eWrite
};

class RAY_CORE_API PlatformFS
{
public:
	static void Initialize();
	static void* OpenFile(FileOpenMode openMode, pcstr path);
};
}
