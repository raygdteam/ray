#pragma once

#include "engine/world/world_render_data.hpp"
#include <renderer_core/command_context.hpp>

namespace ui
{
	class object
	{
	public:
		object();
		~object() {};

		virtual void process() = 0;
		virtual void render() = 0;

	protected:
		StaticQuadSceneProxy* render_data;
		ray::renderer_core_api::GraphicsContext& graphics_context;
	};
}