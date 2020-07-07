#pragma once
#include "resources.hpp"

namespace ray::renderer_core_api::resources
{
	class IIndexBuffer : public IResource
	{
	public:
		virtual ~IIndexBuffer() {}

	};
}
