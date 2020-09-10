#pragma once
#include <core/core.hpp>

class RAY_CORE_API WinApi
{
public:
	static void* CreateEvent(bool manualReset, bool initialState, pcstr name = nullptr);
	static bool SetEvent(void* event);
	static void WaitForEvent(void* event, u32 timeout = 0xFFFFFFFF);
};
