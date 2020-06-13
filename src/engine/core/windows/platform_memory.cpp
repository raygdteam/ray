#include "pch.hpp"
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

void PlatformMemory::GetMemoryStatus(MemoryStatus& Status)
{
	static bool sFirst = true;
	static SYSTEM_INFO sSystemInfo;
	static MEMORYSTATUSEX sMemoryStatus;

	if (sFirst)
	{
		sMemoryStatus.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&sMemoryStatus);
		GetSystemInfo(&sSystemInfo);
		sFirst = false;
	}

	Status.PageSize = sSystemInfo.dwPageSize;
	Status.MinAppAdress = sSystemInfo.lpMinimumApplicationAddress;
	Status.MaxAppAdress = sSystemInfo.lpMaximumApplicationAddress;
	Status.MemoryLoadPercentage = u32(sMemoryStatus.dwMemoryLoad);
	Status.TotalPhysMemory = sMemoryStatus.ullTotalPhys;
	Status.AvailPhysMemory = sMemoryStatus.ullAvailPhys;
}

}


