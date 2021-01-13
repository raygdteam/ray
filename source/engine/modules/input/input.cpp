#include "input.hpp"

#include <windows.h>

Input::Input()
{

}

Input::~Input()
{

}

void Input::unnamed(IPlatformWindow* window)
{
	window->RegisterEventCallback([this](void*, unsigned int message, unsigned long long, signed long long second_parameter) { this->window_event_handler(message, second_parameter); });
}

void Input::window_event_handler(unsigned int message, long long parameter)
{
	this->previous_keys = this->keys;

	this->keys[0] = (GetKeyState(0x46) & 0x8000) != 0;
	this->keys[1] = (GetKeyState(0x1B) & 0x8000) != 0;
}