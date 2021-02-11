#pragma once
#include <core/core.hpp>
#include <core/lib/string.hpp>

class IFile;

enum ProjectFileVersion
{
	eInvalid = 0,
	eInitial = 1,

	eMaxCompatible = eInitial,
	eMax
};

struct Project
{
	/**
	 * Project file version number.
	 */
	ProjectFileVersion Version;

	/**
	 * Engine which was used to edit the project. Will show a warning when opening with newer engine.
	 */
	u32 EngineVersion;
	
	String Name;
};

class ProjectManager
{
	bool SanityCheckProject(Project& project);
	bool IsProjectCompatible(ProjectFileVersion version);
	Project* LoadProjectInternal(IFile* file);
public:
	void LoadProject(String& path);
};

