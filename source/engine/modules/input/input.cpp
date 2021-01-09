#include "input.hpp"
#include <cstdio>
#include <core/os/miniwin.hpp>

void InputBase::WindowEventHandler(u32 msg, s64 rparam)
{
	if (msg != 0x0200) return;
	
	FVector2 currentPosition = { f32((u64)rparam & 0xffff), f32(((u64)rparam >> 16) & 0xffff) };

	//printf("x = %f, y = %f\n", currentPosition.x, currentPosition.y);
	
	_currentDelta = { currentPosition.x - _lastMousePos.x, currentPosition.y - _lastMousePos.y };
	_lastMousePos = currentPosition;
	wasTick = true;
}

InputBase::InputBase()
{
	Array<RawInputDeviceList> devices = WinApi::GetRawInputDeviceList();
	for (RawInputDeviceList& device : devices)
	{
		(void)device;
		//printf("--- TYPE: %lu\n", device.Type);
	}
}

void InputBase::Initialize(IPlatformWindow* window)
{
	window->RegisterEventCallback([this](void*, u32 msg, u64, s64 rparam) { this->WindowEventHandler(msg, rparam); });
}

void InputBase::Reset()
{
	if (wasTick)
	{
		wasTick = false;
		return;
	}
	_currentDelta = FVector2 { 0.0f, 0.0f };
	printf("reset\n");
}
