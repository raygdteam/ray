#include "file_system.hpp"

#include <map>

#include "Shlwapi.h"

namespace ray::file_system
{
    std::map<std::string, std::string> mounted_directories;

    void mount_directory(const char* virtual_directory, const char* physical_directory)
    {
        mounted_directories[virtual_directory] = physical_directory;
    }

    void unmount_directory(const char* virtual_directory)
    {
        mounted_directories.erase(virtual_directory);
    }

#if _WIN32
    bool file_exists(std::string file_path)
    {
        if (!PathFileExists(file_path.c_str()))
        {
            std::string directory;
            std::string file;

            if (std::string::npos != file_path.rfind('/'))
            {
                directory = file_path.substr(0, file_path.rfind('/') + 1);
                file = file_path.substr(directory.size());
            }

            std::string resolved_file_path = mounted_directories[directory] + file;

            return PathFileExists(resolved_file_path.c_str());
        }
        else
            return true;
    }

    bool directory_exists(std::string directory_path)
    {
        if (!PathIsDirectory(directory_path.c_str()))
        {
            std::string path;
            std::string directory;

            if (std::string::npos != directory_path.rfind('/'))
            {
                path = directory_path.substr(0, directory_path.rfind('/') + 1);
                directory = directory_path.substr(path.size());
            }

            std::string resolved_directory_path = mounted_directories[path] + directory;

            return PathIsDirectory(resolved_directory_path.c_str());
        }
        else
            return true;
    }
#endif
};