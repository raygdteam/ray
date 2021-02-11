#pragma once
#include "project.hpp"
#include <core/file_system/file.hpp>

bool ProjectManager::SanityCheckProject(Project& project)
{
	if (project.Version >= ProjectFileVersion::eMax)
		return false;
	
	return true;
}

bool ProjectManager::IsProjectCompatible(ProjectFileVersion version)
{
	return version <= ProjectFileVersion::eMaxCompatible;
}

Project* ProjectManager::LoadProjectInternal(IFile* file)
{
	ProjectFileVersion version = {};
	file->Read(version);
	if (!IsProjectCompatible(version))
		return nullptr;
	
	Project project = { version };
	file->Read(project.EngineVersion);

	u64 nameSize = 0;
	file->Read(nameSize);
	project.Name.resize(nameSize);
	file->Read((u8*)project.Name.AsRawStr(), nameSize);
}

void ProjectManager::LoadProject(String& path)
{
	
}
