#pragma once
#include "ray_renderer_core_base.hpp"

// defines which data will be access by shader
namespace ray::renderer_core_api
{
	struct RootSignatureDesc
	{
		u32 _num_params;
		//IRootParameter* _params;
		u32 _num_static_samplers;
		//StaticSamplerDesc*
		//RootSignatureFlags 
	};

	class IRootSignature : public IRRCBase
	{
	public:
		virtual ~IRootSignature() {}


	};
}