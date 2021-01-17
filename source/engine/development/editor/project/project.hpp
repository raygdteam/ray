#pragma once
#include <core/core.hpp>
#include <core/lib/string.hpp>

enum ProjectFileVersion
{
	Invalid = 0,
	Initial = 1,
	
	Max
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
public:
	void LoadProject(String& path);
};

