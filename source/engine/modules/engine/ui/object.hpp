#pragma once

#include "engine/world/components/transform.hpp"
#include "engine/world/world_render_data.hpp"

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
		Transform* transform;
		RenderData* render_data;
	};
}