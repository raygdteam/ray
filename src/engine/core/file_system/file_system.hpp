#pragma once
#include "core/pch.hpp"
#include "core/core.hpp"
#include "core/serialization/archive.hpp"

namespace ray 
{

struct IFile
{
	size_t size;
	void* map;
};

class RAY_CORE_API FileSystem
{
	FileSystem() = default;
public:
	static FileSystem Get();

	void initialize();
	void Mount(pcstr real, pcstr virtualPath);

	IFile ReadFile(pcstr path);
	bool WriteFile(pcstr path, IFile file);
};

}
