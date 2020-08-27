#include <core/core.hpp>
#include "file_system.hpp"
#include <cstdio>

extern "C"
{
#include <kernel/kernel_file.h>
}

FileSystem::FileSystem()
{
	palFileSysSwitchToExeDirectory();
}

/**
 * NOTE:	i'm using c lib because it makes it come together nicely. Ideally,
 *			we should rewrite it to pal
 */
class NativeFile : public IFile
{
	FILE* _file = nullptr;
	FileMode _mode;
	u64 _size = 0;
public:
	
	NativeFile(FILE* file, FileMode mode)
	{
		_file = file;
		_mode = mode;

		NativeFile::Seek(0, End);
		_size = NativeFile::Tell();
		NativeFile::Seek(0, Begin);
	}
	
	u64 Size() override
	{
		return _size;
	}
	
	bool IsReadOnly() const override
	{
		return !(_mode & FileMode::Write);
	}
	
	void Close() override
	{
		fclose(_file);
		_file = nullptr;
	}
	
	u64 Seek(u64 offset, SeekOrigin origin) override
	{
		return fseek(_file, offset, origin);
	}
	
	u64 Tell() override
	{
		return ftell(_file);
	}
	
	u64 Read(u8* buffer, u64 size) override
	{
		return fread(buffer, 1, size, _file);
	}
	
	u64 Write(void* buffer, u64 size) override
	{
		return fwrite(buffer, size, 1, _file);
	}
};

IFile* FileSystem::OpenFile(pcstr name, FileMode mode)
{
	pcstr mode_ = "";
	switch (mode)
	{
	case Read:
		mode_ = "r";
		break;
	case ReadWrite:
		mode_ = "r+";
		break;
	case Write:
		mode_ = "w";
		break;
	case Append:
		mode_ = "a";
		break;
	case ReadBinary:
		mode_ = "rb";
		break;
	default:
		// todo: unreachable code
		__debugbreak();
	}
	
	FILE* file = fopen(name, mode_);
	if (file == nullptr) return nullptr;
	
	return new NativeFile(file, mode);
}

