#include "input.hpp"

#include <Windows.h>
#include <unordered_set>

namespace ray::input
{
	
std::unordered_set<ray::input::Listener*> gSetListeners;
POINT gOldMousePosition = {};

bool gKeysState[256] = {};
bool gOldKeysState[256] = {};

void Initialize()
{
	for (u16 i = 'A'; i < 'Z'; ++i)
	{
		gKeysState[i] = gOldKeysState[i] = GetKeyState(i) & 0x8000;
	}

	GetCursorPos(&gOldMousePosition);
}

void FireEvent_Keyboard(u16 key, bool state);

void update()
{
	/* 1. Update keyboard state. */
	for (u16 i = 'A'; i < 'Z'; ++i)
	{
		gKeysState[i] = GetKeyState(i) & 0x8000;

		if (gKeysState[i] != gOldKeysState[i])
		{
			FireEvent_Keyboard(i, gKeysState[i]);
			gOldKeysState[i] = gKeysState[i];
		}
	}

	
}

void FireEvent_Keyboard(u16 key, bool state)
{
	for (auto& listener : gSetListeners)
		if (state) listener->OnKeyDown(key);
		else listener->OnKeyUp(key);
}

void AddListener(ray::input::Listener* listener)
{
	gSetListeners.insert(listener);
}

void RemoveListeners(ray::input::Listener* listener)
{
	gSetListeners.erase(listener);
}
	
}