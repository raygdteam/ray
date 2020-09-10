#include "miniwin.hpp"

#include <Windows.h>
#undef CreateEvent

void* WinApi::CreateEvent(bool manualReset, bool initialState, pcstr name)
{
	return CreateEventA(nullptr, manualReset, initialState, name);
}

bool WinApi::SetEvent(void* event)
{
	return bool(::SetEvent(event));
}

void WinApi::WaitForEvent(void* event, u32 timeout)
{
	WaitForSingleObject(event, timeout);
}
