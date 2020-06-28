#pragma once
#include "ray_renderer_core_base.hpp"

// defines which data will be access by shader
namespace ray::renderer_core_api
{
	class IRootSignature : public IRRCBase
	{
	public:
		virtual ~IRootSignature() {}


	};
}