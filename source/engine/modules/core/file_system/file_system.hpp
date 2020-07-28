#pragma once

#include <core/core.hpp>

namespace ray
{

class RAY_CORE_API FileSystem
{
public:
	void MountDirectory(pcstr virtualDirectory, pcstr physicalDirectory);
	void UnmountDirectory(pcstr virtualDirectory);
};

}