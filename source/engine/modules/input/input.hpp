#pragma once

#include <input/input_def.hpp>

#include <array>

#include "app_framework/base/platform_window.hpp"

class RAY_INPUT_API Input
{
public:
	enum key_code
	{
		f,
		escape
	};

	Input();
	~Input();

	void unnamed(IPlatformWindow* window);

	bool get_key_up(const key_code key) { return !keys[key] && previous_keys[key]; }
	bool get_key_down(const key_code key) { return keys[key] && !previous_keys[key]; }

private:
	void window_event_handler(unsigned int, long long);

	std::array<bool, 2> keys;
	std::array<bool, 2> previous_keys;
};
