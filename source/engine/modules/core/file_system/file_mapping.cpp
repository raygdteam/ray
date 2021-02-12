#include "file_mapping.hpp"

#include <windows.h>

u8* MapFile(const char* path)
{
	auto file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
	{
		CloseHandle(file);
		return NULL;
	}

	auto map = CreateFileMapping(file, NULL, PAGE_READONLY, 0, GetFileSize(file, NULL), NULL);

	CloseHandle(file);

	if (!map)
	{
		CloseHandle(map);
		return NULL;
	}

	auto data = (u8*)MapViewOfFile(map, FILE_MAP_READ, 0, 0, 1);

	CloseHandle(map);

	return data;
}

bool UnmapFile(char* data)
{
	return UnmapViewOfFile(data);
}