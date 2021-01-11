#pragma once
#include <core/core.hpp>
#include <input/input_def.hpp>

#include <app_framework/base/platform_window.hpp>
#include <core/lib/delegate.hpp>
#include <core/math/vector.hpp>
#include <core\lib\string.hpp>

enum MouseAxis : u8
{
	eX = 0b01,
	eY = 0b10
};

class RAY_INPUT_API InputBase
{
protected:
	MulticastDelegate<void(u32, u32)> _mouseUpdate;
	FVector2 _lastMousePos;
	FVector2 _currentDelta;

	bool wasTick = true;

	void WindowEventHandler(u32 msg, s64 rparam);
public:
	InputBase();

	void Initialize(IPlatformWindow* window);
	void Reset();

	const FVector2& GetMousePosition()
	{
		return _lastMousePos;
	}
	
	const FVector2& GetMouseDelta()
	{
		return _currentDelta;
	}
};

class Input
{
	void RegisterListener(struct ListenerDescription* listener);

	void CreateMouseInputMapping(String& name, MouseAxis axis);
	
};
