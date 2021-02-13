#include <cstdio>
#include <windows.h>

#include <core/core.hpp>
#include <core/debug/assert.hpp>

#include "file_system.hpp"

class MappedFile : public IFile {
	FileMode _mode;
	HANDLE _file;
	u8* _base;
	u8* _end;
	u8* _current;
public:
	MappedFile(pcstr path, FileMode mode) : _mode(mode)
	{
		_file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		/*if (!file)
			?*/

		u64 size = GetFileSize(_file, NULL);

		/*if (!size)
			?*/

		auto map = CreateFileMapping(_file, NULL, PAGE_READONLY, 0, size, NULL);

		/*if (!map)
			?*/

		this->_base = (u8*)MapViewOfFile(map, FILE_MAP_READ, 0, 0, 1);
		this->_end = this->_base + size;

		this->_current = this->_base;

		/*if (!_base/!_end/!_current)
			?*/
	}

	u64 Size() override
	{
		return this->_end - this->_base;
	}

	bool IsReadOnly() const override
	{
		return !(_mode & FileMode::Write);
	}

	void Close() override
	{
		UnmapViewOfFile(this->_base);
		CloseHandle(this->_file);
	}

	/*u64 Seek(u64 offset, SeekOrigin origin) override
	{
		switch (origin)
		{
		case Begin:
			this->_current = this->_base + offset;
			break;
		case Current:
			this->_current += offset;
			break;
		case End:
			this->_current = this->_end - offset;
			break;
		default:
			//?
			break;
		}

		return ?
	}*/

	u64 Tell() override
	{
		return this->_current - this->_base;
	}

	u64 Read(u8* buffer, u64 size) override
	{
		check(this->_current + size < this->_end);
		memcpy(buffer, this->_current, size);

		this->_current += size;

		return size;
	}
	
	u64 Write(void* buffer, u64 size) override
	{
		return size;
	}
};