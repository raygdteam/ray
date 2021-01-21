#pragma once

#include "input_def.hpp"

#include "core/lib/array.hpp"
#include "core/math/vector.hpp"

#include "app_framework/base/platform_window.hpp"

class RAY_INPUT_API Input
{
	bool _keys[256];
	//bool _previousKeys[256];

	void WindowEventHandler(u32/*, u64*/, s64);
public:
	enum KeyCode
	{
		eA = 'A',
		eD = 'D',
		eS = 'S',
		eW = 'W',
	};
	
	Input();
	~Input();

	void Initialize(IPlatformWindow*);

	bool IsKeyUp(const KeyCode key) { return !_keys[key]; }
	bool IsKeyDown(const KeyCode key) { return _keys[key]; }
};