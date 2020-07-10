#pragma once
#include <core/core.hpp>


namespace ray::file_system
{
	
class RAY_CORE_API FileSystem
{
public:
	void MountDirectory(pcstr virtualDirectory, pcstr physicalDirectory);
	void UnmountDirectory(pcstr virtualDirectory);

	bool FileExists(pcstr filePath);
	bool DirectoryExists(pcstr directoryPath);
};

}
