#include "pch.hpp"
#include "file_system.hpp"

namespace ray
{

void FileSystem::initialize()
{

}

void FileSystem::mount(pcstr real_path, pcstr virtual_path)
{

}

serialization::IArchive FileSystem::ReadFile(pcstr path)
{
	return *(serialization::IArchive*)nullptr;
}


FileSystem FileSystem::Get()
{
	static FileSystem fileSystem;
	return fileSystem;
}

}
