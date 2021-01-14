#include "input.hpp"

#include <windows.h>
#include <iostream>

Input::Input()
{
	this->_keys = new bool[256];
	this->_previousKeys = new bool[256];
}

Input::~Input()
{
	delete this->_keys;
	delete this->_previousKeys;
}

void Input::RegisterWindowEventHandler(IPlatformWindow* window)
{
	RAWINPUTDEVICE rid[1] = {};

	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x06;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = static_cast<HWND>(window->GetWindowHandleRaw());

	RegisterRawInputDevices(rid, 1, sizeof(rid[0]));

	window->RegisterEventCallback([this](void*, u32 message, u64 wParameter, s64 lParameter) { this->WindowEventHandler(message, wParameter, lParameter); });
}

void Input::WindowEventHandler(u32 message, u64 wParameter, s64 lParameter)
{
	(void)message;(void)wParameter;(void)lParameter;

	this->_previousKeys = this->_keys;
}