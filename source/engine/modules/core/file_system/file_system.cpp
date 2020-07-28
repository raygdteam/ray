#include "file_system.hpp"

namespace ray
{

struct MountData
{
    pcstr VirtualPath;
    pcstr PhysicalPath;
};

void FileSystem::MountDirectory(pcstr virtualDirectory, pcstr physicalDirectory)
{
}

void FileSystem::UnmountDirectory(pcstr virtualDirectory)
{
}


};