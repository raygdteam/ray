#pragma once

#include <core/core.hpp>

#include "input_def.hpp"

namespace input
{
	bool RAY_INPUT_API initialize();
	void RAY_INPUT_API update(long x, long y);

	namespace mouse
	{
		long RAY_INPUT_API x();
		long RAY_INPUT_API y();
	}
}
