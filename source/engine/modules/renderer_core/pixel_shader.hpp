#pragma once
#include "shader.hpp"

namespace ray::renderer_core_api
{
	class IPixelShader : public IShader
	{
	public:
		virtual ~IPixelShader() {}

		virtual bool Compile(pcstr path) override { return false; }
		virtual bool LoadBytecode(pcstr path) override { return false; }
	};
}