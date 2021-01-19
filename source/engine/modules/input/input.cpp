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
	/*switch (message)
	{
	case WM_INPUT:
		u32 dwSize;

		GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));

		LPBYTE bytes = new BYTE[dwSize];

		if (!bytes || GetRawInputData((HRAWINPUT)lParameter, RID_INPUT, bytes, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			return;

		RAWINPUT* input = (RAWINPUT*)bytes;

		switch (input->header.dwType)
		{
		case RIM_TYPEKEYBOARD:
			switch (input->data.keyboard.Message)
			{
			case WM_KEYDOWN:
				this->_keys[input->data.keyboard.MakeCode] = true;
			case WM_KEYUP:
				this->_keys[input->data.keyboard.MakeCode] = false;
			}
		}

		delete[] bytes;
	} тут все работает но я подумал а зачем сейчас вообще нужен input и решил это все пока что заккоментировать */
}