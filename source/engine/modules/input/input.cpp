#include "input.hpp"

#include <windows.h>

Input::Input()
{

}

Input::~Input()
{

}

void Input::RegisterWindowEventHandler(IPlatformWindow* window)
{
	window->RegisterEventCallback([this](void*, u32, u64, s64) { this->WindowEventHandler(); });
}

void Input::WindowEventHandler()
{
	this->_previousKeys = this->_keys;

	//this->_keys[] = (GetKeyState() & 0x8000) != 0;
}