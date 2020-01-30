#pragma once
#include "pch.hpp"
#include "core.hpp"
#include "serialization/archive.hpp"

namespace ray 
{

class FileSystem
{
	FileSystem() = default;
public:
	static FileSystem Get();

	void initialize();
	void mount(pcstr real_path, pcstr virtual_path);
	serialization::IArchive ReadFile(pcstr path);
};

}
