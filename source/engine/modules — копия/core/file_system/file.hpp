#pragma once
#include <core/core.hpp>

enum SeekOrigin
{
	Begin = 0,
    Current,
    End,
};

class IFile
{
    friend class FileSystem;
public:
    IFile() = default;
    virtual ~IFile() = default;

	/**
	 * Returns file size.
	 */
    virtual u64 Size() = 0;

	/**
	 * Check if we can write to this file.
	 */
    virtual bool IsReadOnly() const = 0;

	/**
	 * Close the file.
	 * Note: you must delete the IFile.
	 */
    virtual void Close() = 0;

	/**
	 * Move file cursor to *offset* relative to *origin*
	 */
    virtual u64 Seek(u64 offset, SeekOrigin origin) = 0;

	/**
	 * Get the current cursor position.
	 */
    virtual u64 Tell() = 0;

	// NOTE: you must allocate the buffer.
    virtual u64 Read(u8* buffer, u64 size) = 0;
    virtual u64 Write(void* buffer, u64 size) = 0;

    template<typename T>
    bool Read(T& value)
    {
        return (Read((u8*)&value, sizeof(value)) == sizeof(value));
    }

    template<typename T>
    u64 Write(const T& value)
    {
        return (Write((void*)&value, sizeof(value)) == sizeof(value));
    }
};
