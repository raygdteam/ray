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

	bool GetKeyUp(const KeyCode key) { return !_keys[key] && _previousKeys[key]; }
	bool GetKeyDown(const KeyCode key) { return _keys[key] && !_previousKeys[key]; }

private:
	void WindowEventHandler();

	Array<bool> _keys;
	Array<bool> _previousKeys;
};