#include "pch.hpp"
#include "file_system.hpp"
#include "platform/platform_fs.hpp"

namespace ray
{

void FileSystem::initialize()
{
	core::platform::PlatformFS::Initialize();
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
