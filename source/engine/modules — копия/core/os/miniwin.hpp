#pragma once
#include <core/core.hpp>
#include <core/lib/array.hpp>

// задолбали правда
#undef CreateEvent

class RAY_CORE_API WinApi
{
public:
	static void* CreateEvent(bool manualReset, bool initialState, pcstr name = nullptr);
	static bool SetEvent(void* event);
	static void WaitForEvent(void* event, u32 timeout = 0xFFFFFFFF);

	static Array<struct RawInputDeviceList> GetRawInputDeviceList();
};

enum RawInputDeviceType : unsigned long
{
	eMouse,
	eKeyboard,
	eHid
};

struct RawInputDeviceList
{
	void* Handle;
	RawInputDeviceType Type;
};

static_assert(sizeof(RawInputDeviceList) == 16, "size wrong");
