#pragma once

#include "object.hpp"
#include <renderer_core/renderer_2d.hpp>

#include <input/input.hpp>

namespace ui
{
	class button : protected object<bool>
	{
		button() {}
		~button() {}

		virtual bool process();
	};
}
