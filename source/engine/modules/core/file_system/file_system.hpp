#pragma once
#include <core/core.hpp>
#include <core/file_system/file.hpp>

enum FileMode
{
    Read = 0x01,
    Write = 0x02,
    ReadWrite = Read | Write,
    Append = 0x04,
	WriteBinary,
	ReadBinary,
};

class RAY_CORE_API FileSystem
{
public:
	FileSystem();

	bool Initialize() const;
	
	/*
	 * Open the file.
	 * Path is relative to the folder .exe is in.
	 * For instance, '../../ray.sln' will resolve to
	 * 'ray/.build/bin/../../ray.sln' ==> 'ray/ray.sln'
	 *
	 * NOTE: FileMode#Write will erase your file contents if it exists!!!
	 * NOTE: You must call IFile#Close and delete the instance yourself!!
	 */
	IFile* OpenFile(pcstr name, FileMode mode);

	/**
	 * Create all directories that is missing
	 */
	void CreateAllDirectories(pcstr path);
};

extern RAY_CORE_API FileSystem gFileSystem;
