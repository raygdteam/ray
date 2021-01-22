#include <core/core.hpp>
#include "input.hpp"
#include <windows.h>
#include "core/log/log.hpp"

Input::Input()
{

}

Input::~Input()
{

}

Logger* gLog;

void Input::Initialize(IPlatformWindow* window)
{
	gLog = new Logger("Input");
	
	RAWINPUTDEVICE rid[2];

	rid[0].usUsagePage = 0x01;
	rid[0].usUsage = 0x02;
	rid[0].dwFlags = 0;
	rid[0].hwndTarget = 0;

	rid[1].usUsagePage = 0x01;
	rid[1].usUsage = 0x06;
	rid[1].dwFlags = 0;
	rid[1].hwndTarget = 0;

	if (RegisterRawInputDevices(rid, 2, sizeof(rid[0])) == FALSE)
		return;

	window->RegisterEventCallback([this](void*, u32 message, u64/* wParameter*/, s64 lParameter) { this->WindowEventHandler(message/*, wParameter*/, lParameter); });
}

void Input::WindowEventHandler(u32 message/*, u64 wParameter*/, s64 lParameter)
{
	if (message != WM_INPUT) return;

	u32 dwSize;

	GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

	u8 data[dwSize];

	if (GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, data, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
		return;

	RAWINPUT* input = reinterpret_cast<RAWINPUT*>(data);

	if (input->header.dwType != RIM_TYPEKEYBOARD)
		return;

	RAWKEYBOARD& keyboard = input->data.keyboard;
	
	if (keyboard.Message == WM_KEYDOWN)
	{
		gLog->Log("WM_KEYDOWN: %u", keyboard.VKey);
		this->_keys[keyboard.VKey] = true;
	}
	else if(keyboard.Message == WM_KEYUP)
	{
		gLog->Log("WM_KEYUP: %u", keyboard.VKey);
		this->_keys[keyboard.VKey] = false;
	}
	
}