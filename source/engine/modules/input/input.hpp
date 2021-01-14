#pragma once

#include "input_def.hpp"

#include "core/lib/array.hpp"
#include "core/math/vector.hpp"

#include "app_framework/base/platform_window.hpp"

class RAY_INPUT_API Input
{
public:
	enum KeyCode
	{

	};
	
	Input();
	~Input();

	void RegisterWindowEventHandler(IPlatformWindow* window);

	bool IsKeyUp(const KeyCode key) { return !_keys[key] && _previousKeys[key]; }
	bool IsKeyDown(const KeyCode key) { return _keys[key] && !_previousKeys[key]; }

private:
	void WindowEventHandler(u32, u64, s64);

	bool* _keys;
	bool* _previousKeys;
};