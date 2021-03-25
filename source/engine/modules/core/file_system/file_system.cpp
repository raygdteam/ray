#include <core/core.hpp>
#include <filesystem>
#include "file_system.hpp"
#include <cstdio>

extern "C"
{
#include <kernel/kernel_file.h>
}

RAY_CORE_API FileSystem gFileSystem;

FileSystem::FileSystem()
{
	
}

bool FileSystem::Initialize() const
{
	palFileSysSwitchToExeDirectory();
	return true;
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
		return !(_mode & FileMode::eWrite);
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
	case eRead:
		mode_ = "r";
		break;
	case eReadWrite:
		mode_ = "r+";
		break;
	case eWrite:
		mode_ = "w";
		break;
	case eAppend:
		mode_ = "a";
		break;
	case eReadBinary:
		mode_ = "rb";
		break;
	case eWriteBinary:
		mode_ = "wb";
		break;
	default:
		// todo: unreachable code
		__debugbreak();
	}
	
	FILE* file = fopen(name, mode_);
	if (file == nullptr) return nullptr;
	
	return new NativeFile(file, mode);
}

void FileSystem::CreateAllDirectories(pcstr path)
{
	(void)path;
}

void FileSystem::ReadTextFile(const String& path, String& trg)
{
	IFile* file = gFileSystem.OpenFile(path.AsRawStr(), eRead);
	trg.resize(file->Size());
	file->Read((u8*)trg.data(), file->Size());
	file->Close();
	delete file;
}

void FileSystem::ListFilesRecursively(String path, Array<String>& out)
{
	std::filesystem::recursive_directory_iterator it(std::filesystem::path(path.AsRawStr()));

	for (const std::filesystem::directory_entry& directory : it)
	{
		std::filesystem::path path = directory.path();
		
		if (directory.is_directory()) continue;
		
		String res ((char*)path.u8string().c_str());
		while(res.find_first_of("\\") != String::npos)
		{
			res.replace(res.find_first_of("\\"), 1, "/", 1);
		}
		out.PushBack(res);
	}
}

