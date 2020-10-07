#pragma once

#include <core/core.hpp>

#include "input_def.hpp"

namespace input
{
	bool RAY_INPUT_API initialize();
	void __declspec(dllexport) update(long x, long y);

	namespace mouse
	{
		long __declspec(dllexport) x();
		long __declspec(dllexport) y();
	}
}
