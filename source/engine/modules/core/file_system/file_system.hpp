#pragma once
#include <core/core.hpp>
#include <core/file_system/file.hpp>

enum FileMode
{
    Read = 0x01,
    Write = 0x02,
    ReadWrite = Read | Write,
    Append = 0x04,
	ReadBinary,
};

class RAY_CORE_API FileSystem
{
public:
	FileSystem();

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
};
