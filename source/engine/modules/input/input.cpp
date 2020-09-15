#include "input.hpp"

#ifdef RAY_PLATFORM_WIN

#include <windows.h>

namespace Ray
{
	Input::Input()
	{
		POINT unnamed;

		GetCursorPos(&unnamed);

		this->mouse.coordinates.x = unnamed.x;
		this->mouse.coordinates.x = unnamed.y;

	}

	void Input::Update(u16& x, u16& y)
	{
		if (this->mouse.coordinates.x != x || this->mouse.coordinates.y != y)
		{
			this->mouse.coordinates.x = x;
			this->mouse.coordinates.y = y;
		}
	}
}

#endif
