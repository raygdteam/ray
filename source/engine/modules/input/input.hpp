#pragma once

#include <input/input_def.hpp>

#include <array>

#include "app_framework/base/platform_window.hpp"

#include "core/math/vector.hpp"

class RAY_INPUT_API Input
{
public:
	enum key_code
	{
		f,
		escape,
		mouse_left,
		mouse_middle,
		mouse_right
	};

	Input();
	~Input();

	void Initialize(IPlatformWindow* window);

	bool get_key_up(const key_code key) { return !keys[key] && previous_keys[key]; }
	bool get_key_down(const key_code key) { return keys[key] && !previous_keys[key]; }

	FVector2 mouse_position;
	FVector2 mouse_delta;

private:
	void window_event_handler(void*, unsigned int, unsigned long long, signed long long);

	std::array<bool, 5> keys;
	std::array<bool, 5> previous_keys;

	float mouse_wheel_delta = 0;
};
