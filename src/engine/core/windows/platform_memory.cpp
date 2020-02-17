#include "pch.hpp"
#include "core.hpp"
#include "platform/platform_memory.hpp"
#include <ray/os/include.hpp>

namespace ray::core::platform
{

void* PlatformMemory::LowLevelAlloc(size_t size)
{
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
}

void PlatformMemory::LowLevelFree(void* data)
{
	HeapFree(GetProcessHeap(), 0, data);
}

void PlatformMemory::Memcpy(void* from, void* to, size_t size)
{
	for (u64 i = 0; i < size; ++i)
	{
		*(void**)((u64*)to + i) = *(void**)((u64*)from + i);
	}
}

void PlatformMemory::Memset(void* data, u8 value, size_t size)
{
	auto p = (unsigned char*)data;
	while (size > 0)
	{
		*p = value;
		p++;
		size--;
	}
}

u32 PlatformMemory::GetPageSize()
{
	static bool sFirst = true;
	static SYSTEM_INFO sSystemInfo;

	if (sFirst)
	{
		GetSystemInfo(&sSystemInfo);
		sFirst = false;
	}
	return static_cast<u32>(sSystemInfo.dwPageSize);
}

void* PlatformMemory::GetMinAppAdress()
{
	static bool sFirst = true;
	static SYSTEM_INFO sSystemInfo;

	if (sFirst)
	{
		GetSystemInfo(&sSystemInfo);
		sFirst = false;
	}
	return static_cast<void*>(sSystemInfo.lpMinimumApplicationAddress);
}

void* PlatformMemory::GetMaxAppAdress()
{
	static bool sFirst = true;
	static SYSTEM_INFO sSystemInfo;

	if (sFirst)
	{
		GetSystemInfo(&sSystemInfo);
		sFirst = false;
	}
	return static_cast<void*>(sSystemInfo.lpMaximumApplicationAddress);
}

u16 PlatformMemory::GetMemoryLoadPercentage()
{
	static MEMORYSTATUSEX sMemStatus;
	static bool sFirst = true;

	if (sFirst)
	{
		sMemStatus.dwLength = sizeof(sMemStatus);
		GlobalMemoryStatusEx(&sMemStatus);
	}

	return static_cast<u16>(sMemStatus.dwMemoryLoad);
}

u64 PlatformMemory::GetTotalPhysMemory()
{
	static MEMORYSTATUSEX sMemStatus;
	static bool sFirst = true;

	if (sFirst)
	{
		sMemStatus.dwLength = sizeof(sMemStatus);
		GlobalMemoryStatusEx(&sMemStatus);
	}

	return static_cast<u64>(sMemStatus.ullTotalPhys);
}

u64 PlatformMemory::GetAvailPhysMemory()
{
	static MEMORYSTATUSEX sMemStatus;
	static bool sFirst = true;

	if (sFirst)
	{
		sMemStatus.dwLength = sizeof(sMemStatus);
		GlobalMemoryStatusEx(&sMemStatus);
	}

	return static_cast<u64>(sMemStatus.ullAvailPhys);
}

}


