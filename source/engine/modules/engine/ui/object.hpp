#pragma once

#include "engine/world/world_render_data.hpp"
#include <renderer_core/command_context.hpp>

namespace ui
{
	template <typename type>
	class object
	{
	public:
		object();
		~object();

		virtual type process() = 0;

	protected:
		StaticQuadSceneProxy* render_data;
		ray::renderer_core_api::GraphicsContext& graphics_context;
	};
}