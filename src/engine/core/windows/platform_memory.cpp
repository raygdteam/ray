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

namespace windows
{
	MEMORYSTATUSEX& GetMemoryStatus()
	{
		static MEMORYSTATUSEX sMemStatus;
		static bool sFirst = true;

		if (sFirst)
		{
			sMemStatus.dwLength = sizeof(sMemStatus);
			GlobalMemoryStatusEx(&sMemStatus);
		}

		return sMemStatus;
	}

	SYSTEM_INFO& GetSystemInfo()
	{
		static bool sFirst = true;
		static SYSTEM_INFO sSystemInfo;

		if (sFirst)
		{
			GetSystemInfo(&sSystemInfo);
			sFirst = false;
		}

		return sSystemInfo;
	}
}

u32 PlatformMemory::GetPageSize()
{
	return static_cast<u32>(windows::GetSystemInfo().dwPageSize);
}

void* PlatformMemory::GetMinAppAdress()
{
	return static_cast<void*>(windows::GetSystemInfo().lpMinimumApplicationAddress);
}

void* PlatformMemory::GetMaxAppAdress()
{
	return static_cast<void*>(windows::GetSystemInfo().lpMaximumApplicationAddress);
}

u16 PlatformMemory::GetMemoryLoadPercentage()
{
	return static_cast<u16>(windows::GetMemoryStatus().dwMemoryLoad);
}

u64 PlatformMemory::GetTotalPhysMemory()
{
	return static_cast<u64>(windows::GetMemoryStatus().ullTotalPhys);
}

u64 PlatformMemory::GetAvailPhysMemory()
{
	return static_cast<u64>(windows::GetMemoryStatus().ullAvailPhys);
}

}


