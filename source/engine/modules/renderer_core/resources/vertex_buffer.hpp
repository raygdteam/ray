#pragma once
#include "resources.hpp"


namespace ray::renderer_core_api::resources
{

	class IVertexBuffer : public IResource
	{
	public:
		virtual ~IVertexBuffer() {}

		void SetStride(u32 stride) noexcept { _stride = stride; }
		u32 GetStride() const noexcept { return _stride; }

	protected:
		u32 _stride;

	};
}
