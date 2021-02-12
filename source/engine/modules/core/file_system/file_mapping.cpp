#include "file_mapping.hpp"

#include <windows.h>

char* MapFile(const char* path)
{
	auto file = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE)
		return NULL;

	auto size = GetFileSize(file, NULL);

	if (size == INVALID_FILE_SIZE || size == 0)
	{
		CloseHandle(file);
		return NULL;
	}

	auto map = CreateFileMapping(file, NULL, PAGE_READONLY, 0, size, NULL);

	if (!map)
	{
		CloseHandle(file);
		return NULL;
	}

	auto data = (char*)MapViewOfFile(map, FILE_MAP_READ, 0, 0, size);

	CloseHandle(map);

	return data;
}

bool UnmapFile(char* data)
{
	return UnmapViewOfFile(data);
}