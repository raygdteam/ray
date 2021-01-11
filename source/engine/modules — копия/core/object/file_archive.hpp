#pragma once
#include <core/object/archive.hpp>
#include <core/file_system/file.hpp>

struct FileArchive : public Archive
{
	IFile* file = nullptr;

	void Read(u8* buffer, u64 size) override
	{
		file->Read(buffer, size);
	}

	void Write(void* buffer, u64 size) override
	{
		file->Write(buffer, size);
	}
};

