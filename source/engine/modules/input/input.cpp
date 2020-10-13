#include "input.hpp"

#ifdef RAY_PLATFORM_WIN

#include <windows.h>

POINT coordinates = {};

#include <iostream>

namespace input
{
	bool initialize()
	{
		return GetCursorPos(&coordinates);
	}

	void update(long x, long y)
	{
		coordinates.x = x;
		coordinates.y = y;
	}

	namespace mouse
	{
		long x()
		{
			return coordinates.x;
		}

		long y()
		{
			return coordinates.y;
		}
	}
}

POINT raw_coordinates = {};

namespace raw_input
{
	bool initialize()
	{
		RAWINPUTDEVICE RID[2];

		RID[0].usUsagePage = 0x01;
		RID[0].usUsage = 0x02;
		RID[0].dwFlags = RIDEV_NOLEGACY;
		RID[0].hwndTarget = 0;

		RID[1].usUsagePage = 0x01;
		RID[1].usUsage = 0x06;
		RID[1].dwFlags = RIDEV_NOLEGACY;
		RID[1].hwndTarget = 0;

		GetCursorPos(&raw_coordinates);

		return RegisterRawInputDevices(RID, 2, sizeof(RID[0]));
	}

	void update(long x, long y)
	{
		raw_coordinates.x += x;
		raw_coordinates.y += y;
	}

	namespace mouse
	{
		long x()
		{
			return raw_coordinates.x;
		}

		long y()
		{
			return raw_coordinates.y;
		}
	}
}

#endif
