#pragma once
#include <core/math/vector.hpp>

struct Ui2IO
{
	/**
	 * Mouse position relative to the upper left corner of the window.
	 */
	FVector2 MousePosition;

	/**
	 * Mouse buttons list.
	 * 0    = LMB
	 * 1    = RMB
	 * 2    = Middle
	 * 3, 4 = Additional buttons; often forward/back
	 */
	bool MouseButtons[5];

	/**
	 * Delta of mouse wheel compared to previous frame.
	 */
	f32 MouseWheel;

	bool KeyboardCtrl;
	bool KeyboardShift;
	bool KeyboardAlt;
	bool KeysDown[512];
};
