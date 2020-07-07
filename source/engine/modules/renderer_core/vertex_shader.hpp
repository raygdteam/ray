#pragma once
#include "shader.hpp"

namespace ray::renderer_core_api
{
	class IVertexShader : public IShader
	{
	public:
		virtual	~IVertexShader() {}

		virtual bool Compile(pcstr path) override { return false; }
		virtual bool LoadBytecode(pcstr path) override { return false; }
	};
}
