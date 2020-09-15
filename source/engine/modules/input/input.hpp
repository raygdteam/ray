#pragma once
#include <core/core.hpp>
#include <input/input_def.hpp>

class RAY_INPUT_API Input
{
	struct
	{
		struct
		{
			u16 x = 0;
			u16 y = 0;
		} Coordinates;
	} _mouse;
public:
	Input();

	void Update(u16 x, u16 y);
};
