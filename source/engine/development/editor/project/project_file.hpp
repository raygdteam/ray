#pragma once
#include <core/core.hpp>
#include <core/lib/string.hpp>
#include <core/json/json.hpp>

class IFile;

enum class ProjectFileVersion
{
	eInvalid = 0,
	eInitial = 1,

	eMaxCompatible = eInitial,
	eMax
};

struct ProjectFile
{
	/**
	 * Path to the project folder.
	 */
	String Path;
	
	/**
	 * Project file version number.
	 */
	ProjectFileVersion Version;

	/**
	 * Engine which was used to edit the project. Will show a warning when opening with newer engine.
	 */
	u32 EngineVersion;
	
	String Name;
	String Codename;
	String Description;

	u64 GameVersion;
	
};

class ProjectManager
{
	bool SanityCheckProject(ProjectFile& project);
	bool IsProjectCompatible(ProjectFileVersion version);
	ProjectFile* ReadProjectFile(JsonValue& file);
public:
	ProjectFile* ReadProjectFile(String& path);
};

