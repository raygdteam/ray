#include "input.hpp"

#include <Windows.h>
#include <unordered_set>

namespace ray::input
{
	std::unordered_set<ray::input::listener*> set_listeners;
	POINT old_mouse_position = {};

	unsigned char keys_state[256] = {};
	unsigned char old_keys_state[256] = {};

	bool first_time = true;

	void update()
	{
		POINT current_mouse_position = {};
		::GetCursorPos(&current_mouse_position);

		if (first_time)
		{
			old_mouse_position = current_mouse_position;
			first_time = !first_time;
		}

		if (current_mouse_position.x != old_mouse_position.x|| current_mouse_position.y != old_mouse_position.y)
		{
			std::unordered_set<ray::input::listener*>::iterator it = set_listeners.begin();

			while (it != set_listeners.end())
			{
				(*it)->on_mouse_move(current_mouse_position.x - old_mouse_position.x, current_mouse_position.y - old_mouse_position.y);
				++it;
			}
		}

		old_mouse_position = current_mouse_position;

		if (::GetKeyboardState(keys_state))
		{
			for (unsigned int i = 0; i < 256; i++)
			{
				if (keys_state[i] & 0x80)
				{
					std::unordered_set<ray::input::listener*>::iterator it = set_listeners.begin();

					while (it != set_listeners.end())
					{
						if (i == VK_LBUTTON)
						{
							if (keys_state[i] != old_keys_state[i])
								(*it)->on_left_mouse_down(current_mouse_position.x, current_mouse_position.y);
						}
						else if (i == VK_RBUTTON)
						{
							if (keys_state[i] != old_keys_state[i])
								(*it)->on_right_mouse_down(current_mouse_position.x, current_mouse_position.y);
						}
						else
							(*it)->on_key_down(i);

						++it;
					}
				}
				else
				{
					if (keys_state[i] != old_keys_state[i])
					{
						std::unordered_set<ray::input::listener*>::iterator it = set_listeners.begin();

						while (it != set_listeners.end())
						{
							if (i == VK_LBUTTON)
								(*it)->on_left_mouse_up(current_mouse_position.x, current_mouse_position.y);
							else if (i == VK_RBUTTON)
								(*it)->on_right_mouse_up(current_mouse_position.x, current_mouse_position.y);
							else
								(*it)->on_key_up(i);

							++it;
						}
					}

				}

			}

			::memcpy(old_keys_state, keys_state, sizeof(unsigned char) * 256);
		}
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