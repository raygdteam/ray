#pragma once

#include "object.hpp"
#include <renderer_core/renderer_2d.hpp>

#include <input/input.hpp>

namespace ui
{
	class button : protected object
	{
		button(StaticQuadSceneProxy* render_data, ray::renderer_core_api::GraphicsContext& graphics_context)
		{
			this->render_data = render_data;
			//this->graphics_context = graphics_context;
		}

		//~button() {}

		virtual void process() override;
		virtual void render() override;
	};
}