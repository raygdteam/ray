#pragma once
#include "core/pch.hpp"
#include "core/core.hpp"
#include "core/serialization/archive.hpp"

namespace ray 
{

class RAY_CORE_API FileSystem
{
	FileSystem() = default;
public:
	static FileSystem Get();

	void initialize();
	void mount(pcstr real_path, pcstr virtual_path);
	serialization::IArchive ReadFile(pcstr path);
};

}
