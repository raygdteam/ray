#pragma once
#include "project_file.hpp"
#include <core/file_system/file.hpp>

#include "core/file_system/file_system.hpp"


bool ProjectManager::SanityCheckProject(ProjectFile& project)
{
	if (project.Version >= ProjectFileVersion::eMax)
		return false;
	
	return true;
}

bool ProjectManager::IsProjectCompatible(ProjectFileVersion version)
{
	return version <= ProjectFileVersion::eMaxCompatible;
}

ProjectFile* ProjectManager::ReadProjectFile(JsonValue& file)
{
	ProjectFileVersion version = ProjectFileVersion(file["Version"].AsInt());
	if (!IsProjectCompatible(version)) return nullptr;

	u32 engineVersion = u32(file["EngineVersion"].AsInt());
	// ..

	ProjectFile* project = new ProjectFile();
	project->Version = version;
	project->EngineVersion = engineVersion;
	project->Name = file["Name"].AsString();
	project->Codename = file["Codename"].AsString();
	project->Description = file["Description"].AsString();

	return project;
}

ProjectFile* ProjectManager::ReadProjectFile(String& path)
{
	//String _path = String(path.substr(0, path.find_last_of("/")));
	//String projectFile;
	//gFileSystem.ReadTextFile(path, projectFile);

	//JsonValue projectFileJson = JsonParser::Parse(projectFile);
	//ProjectFile* project = ReadProjectFile(projectFileJson);
	return nullptr;
}
