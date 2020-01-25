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


FileSystem FileSystem::Get()
{
	static FileSystem fileSystem;
	return fileSystem;
}

}
