#include <cstdio>
#include <windows.h>

#include <core/core.hpp>
#include <core/debug/assert.hpp>

#include "file_system.hpp"

class MappedFile : public IFile 
{
private:
	FileMode _mode;
	HANDLE _file;
	HANDLE _fileMapping;
	u8* _buffer;
	u8* _end;
	u8* _current;
	pcstr _path;

public:
	MappedFile() noexcept
		: _file(0)
		, _buffer(nullptr)
		, _end(nullptr)
		, _current(nullptr)
		, _path(nullptr)
	{}

	MappedFile(pcstr path, size_t fileSize, FileMode mode) noexcept
		: _mode(mode)
		, _path(path)
	{
		Create(path, fileSize, mode);
	}

	~MappedFile()
	{
		if (_file != 0 && _buffer != nullptr)
			Close();
	}

public:
	void Create(pcstr path, size_t fileSize, FileMode mode) noexcept;
	void Reset(size_t newSize) noexcept;

	void Close() override
	{
		UnmapViewOfFile(this->_buffer);
		CloseHandle(this->_fileMapping);
		CloseHandle(this->_file);
	}

	u64 Read(u8* buffer, u64 size) override;
	u64 Write(void* buffer, u64 size) override;
	u64 Seek(size_t offset, SeekOrigin origin) override;

public:
	u64 Size() override
	{
		return this->_end - this->_buffer;
	}

	bool IsReadOnly() const override
	{
		return !(_mode & FileMode::eWrite);
	}

	u64 Tell() override
	{
		return this->_current - this->_buffer;
	}

};