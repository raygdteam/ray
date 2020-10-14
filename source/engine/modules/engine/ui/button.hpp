#pragma once

#include "object.hpp"
#include <renderer_core/renderer_2d.hpp>

#include <input/input.hpp>

namespace ui
{
	class button : protected object
	{
		button(StaticQuadSceneProxy* render_data)
		{
			this->render_data = render_data;
		}

		//~button() {}

		virtual void tick() override;
		virtual void render() override;
	};
}