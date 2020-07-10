#include "file_system.hpp"
#include <map>
#include <Shlwapi.h>
#include <string>

namespace ray::file_system
{

struct MountData
{
    pcstr VirtualPath;
    pcstr PhysicalPath;
};

// TODO: rewrite to our containers
std::map<pcstr, pcstr> gMountedDirectories;

void FileSystem::MountDirectory(pcstr virtualDirectory, pcstr physicalDirectory)
{
    gMountedDirectories[virtualDirectory] = physicalDirectory;
}

void FileSystem::UnmountDirectory(pcstr virtualDirectory)
{
    gMountedDirectories.erase(virtualDirectory);
}


#if _WIN32
#pragma comment(lib, "Shlwapi.lib")

bool FileSystem::FileExists(pcstr filePath)
{
    if (!PathFileExists(filePath))
    {
        std::string file_path(filePath);
        std::string directory;
        std::string file;

        if (std::string::npos != file_path.rfind('/'))
        {
            directory = file_path.substr(0, file_path.rfind('/') + 1);
            file = file_path.substr(directory.size());
        }

        std::string resolved_file_path = gMountedDirectories[directory.c_str()] + file;

        return PathFileExists(resolved_file_path.c_str());
    }
    else
        return true;
	
}

bool FileSystem::DirectoryExists(pcstr directoryPath)
{
    if (!PathIsDirectory(directoryPath))
    {
        std::string directory_path(directoryPath);
        std::string path;
        std::string directory;

        if (std::string::npos != directory_path.rfind('/'))
        {
            path = directory_path.substr(0, directory_path.rfind('/') + 1);
            directory = directory_path.substr(path.size());
        }

        std::string resolved_directory_path = gMountedDirectories[path.c_str()] + directory;

        return PathIsDirectory(resolved_directory_path.c_str());
    }
    else
        return true;
}
#endif
};
