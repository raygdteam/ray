#pragma once
#include "ray_renderer_core_base.hpp"

namespace ray::renderer_core_api
{
	//the area to draw in. pixels otside that area won't be drawn onto

	class IRect : public IRRCBase
	{
	public:
		virtual ~IRect() {}
	};
}