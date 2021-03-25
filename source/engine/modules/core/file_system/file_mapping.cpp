#include "file_mapping.hpp"

void MappedFile::Create(pcstr path, size_t fileSize, FileMode mode) noexcept
{
	u32 access;
	switch (mode)
	{
	case eRead:
		access = GENERIC_READ;
		break;
	case eWrite:
		access = GENERIC_WRITE;
		break;
	case eReadWrite:
		access = GENERIC_READ | GENERIC_WRITE;
		break;
	case eAppend:
		access = FILE_APPEND_DATA;
		break;
	case eWriteBinary:
		access = GENERIC_WRITE | GENERIC_EXECUTE;
		break;
	case eReadBinary:
		access = GENERIC_READ | GENERIC_EXECUTE;
		break;
	default:
		break;
	}

	WIN32_FIND_DATA findFileData;
	HANDLE handle = FindFirstFileEx(path, FindExInfoBasic, &findFileData, FindExSearchNameMatch, nullptr, 0);
	bool found = handle != INVALID_HANDLE_VALUE;

	if (found)
	{
		FindClose(handle);
	}

	_file = CreateFile(path, access, 0, nullptr, found ? OPEN_EXISTING : CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	check(_file != INVALID_HANDLE_VALUE);

	u32 currentFileSize = GetFileSize(_file, nullptr);

	if (currentFileSize == 0 || currentFileSize < fileSize)
	{
		currentFileSize = fileSize;
	}

	switch (mode)
	{
	case eRead:
		access = PAGE_READONLY;
		break;
	case eWrite:
	case eReadWrite:
		access = PAGE_READWRITE;
		break;
	case eWriteBinary:
		access = PAGE_EXECUTE_READWRITE;
		break;
	case eReadBinary:
		access = PAGE_EXECUTE_READ;
		break;
	default:
		break;
	}

	_fileMapping = CreateFileMapping(_file, nullptr, access, 0, currentFileSize, nullptr);
	check(_fileMapping != nullptr);

	_buffer = static_cast<u8*>(MapViewOfFile(_fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, currentFileSize));
	check(_buffer != nullptr);

	_current = _buffer;
	_end = _buffer + currentFileSize;
}

void MappedFile::Reset(size_t newSize) noexcept
{
	if (newSize <= Size())
		return;

	BOOL success = FlushViewOfFile(_buffer, 0);
	check(success != FALSE);

	Close();
	Create(_path, newSize, _mode);
}

u64 MappedFile::Read(u8* buffer, u64 size)
{
	check(this->_current + size < this->_end);
	memcpy(buffer, this->_current, size);

	this->_current += size;

	return size;
}

u64 MappedFile::Write(void* buffer, u64 size)
{
	check(this->_current + size < this->_end);
	memcpy(_current, buffer, size);

	_current += size;
	return size;
}

u64 MappedFile::Seek(size_t offset, SeekOrigin origin)
{
	switch (origin)
	{
	case Begin:
		_current = _buffer + offset;
		break;
	case Current:
		_current += offset;
		break;
	case End:
		// ???
		_current = _end - offset;
		break;
	default:
		check(false);
	}

	return Size();
}
