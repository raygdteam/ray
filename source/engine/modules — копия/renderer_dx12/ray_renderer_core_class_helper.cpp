#include "pch.hpp"
#include <renderer_core/ray_renderer_core_class_helper.hpp>

namespace ray::renderer::d3d12
{
	class D3D12RRCClassHelper : public ray::renderer_core_api::IRRCClassHelper
	{
	public:
		

	};
}

/*extern "C" __declspec(dllexport)
IRRCClassHelper* GetRRCClassHelper()
{
	return new ray::renderer::d3d12::D3D12RRCClassHelper;
}*/
