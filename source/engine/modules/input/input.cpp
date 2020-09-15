#include "input.hpp"

#ifdef RAY_PLATFORM_WIN

#include <windows.h>

Input::Input()
{
	POINT mousePos = {};

	GetCursorPos(&mousePos);

	this->_mouse.Coordinates.x = mousePos.x;
	this->_mouse.Coordinates.y = mousePos.y;
}

void Input::Update(u16 x, u16 y)
{
	if (this->_mouse.Coordinates.x != x || this->_mouse.Coordinates.y != y)
	{
		this->_mouse.Coordinates.x = x;
		this->_mouse.Coordinates.y = y;
	}
}

#endif
