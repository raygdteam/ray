#include "input.hpp"

#ifdef RAY_PLATFORM_WIN

#include <windows.h>

#include <iostream>

POINT coordinates = {};

namespace input
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

		GetCursorPos(&coordinates);

		std::cout << coordinates.x << " " << coordinates.y << std::endl;

		return RegisterRawInputDevices(RID, 2, sizeof(RID[0]));
	}

	void update(long x, long y)
	{
		coordinates.x += x;
		coordinates.y += y;
		
		std::cout << coordinates.x << " " << coordinates.y << std::endl;
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

#endif
