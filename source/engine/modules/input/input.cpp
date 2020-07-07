#include "input.hpp"

#include <Windows.h>
#include <unordered_set>

namespace ray::input
{
	
std::unordered_set<ray::input::listener*> set_listeners;
POINT old_mouse_position = {};

bool keys_state[256] = {};
bool old_keys_state[256] = {};

void Initialize()
{
	for (u16 i = 'A'; i < 'Z'; ++i)
	{
		keys_state[i] = old_keys_state[i] = GetKeyState(i) & 0x8000;
	}

	GetCursorPos(&old_mouse_position);
}

void FireEvent_Keyboard(u16 key, bool state);

void update()
{
	/* 1. Update keyboard state. */
	for (u16 i = 'A'; i < 'Z'; ++i)
	{
		keys_state[i] = GetKeyState(i) & 0x8000;

		if (keys_state[i] != old_keys_state[i])
		{
			FireEvent_Keyboard(i, keys_state[i]);
			old_keys_state[i] = keys_state[i];
		}
	}

	
}

void FireEvent_Keyboard(u16 key, bool state)
{
	for (auto& listener : set_listeners)
		if (state) listener->on_key_down(key);
		else listener->on_key_up(key);
}

void add_listener(ray::input::listener* listener)
{
	set_listeners.insert(listener);
}

void remove_listener(ray::input::listener* listener)
{
	set_listeners.erase(listener);
}
	
}