#include "input.hpp"

#include <windows.h>
#include <stdio.h>

Input::Input()
{
	POINT mouse_screen_position;
	if (::GetCursorPos(&mouse_screen_position))
	{
		this->mouse_position = FVector2{ .x = static_cast<float>(mouse_screen_position.x), .y = static_cast<float>(mouse_screen_position.y) };
	}
}

Input::~Input()
{

}

void Input::Initialize(IPlatformWindow* window)
{
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = static_cast<HWND>(window->GetWindowHandleRaw());
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	window->RegisterEventCallback([this](void* window_handle, unsigned int message, unsigned long long wparameter, signed long long lparameter) { this->window_event_handler(window_handle, message, wparameter, lparameter); });
}

void Input::window_event_handler(void* window_handle, unsigned int message, unsigned long long wparameter, signed long long lparameter)
{
	this->mouse_delta.x = 0;
	this->mouse_delta.y = 0;

	this->previous_keys = this->keys;

	if (message == WM_INPUT)
	{
		UINT dwSize = 48;
		static BYTE lpb[48];

		GetRawInputData(reinterpret_cast<HRAWINPUT>(lparameter), RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			this->mouse_delta.x = static_cast<float>(raw->data.mouse.lLastX);
			this->mouse_delta.y = static_cast<float>(raw->data.mouse.lLastY);
		}
	}

	this->mouse_position.x += this->mouse_delta.x;
	this->mouse_position.y += this->mouse_delta.y;

	this->mouse_wheel_delta = (float)GET_WHEEL_DELTA_WPARAM(wparameter) / (float)WHEEL_DELTA;

	this->keys[2] = (GetAsyncKeyState(0x01) & 0x8000) != 0;
	this->keys[3] = (GetAsyncKeyState(0x04) & 0x8000) != 0;
	this->keys[4] = (GetAsyncKeyState(0x02) & 0x8000) != 0;

	this->keys[0] = (GetAsyncKeyState(0x46) & 0x8000) != 0;
	this->keys[1] = (GetAsyncKeyState(0x1B) & 0x8000) != 0;
}