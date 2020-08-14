#pragma once

#include <core/math/vector.hpp>

namespace ray::renderer_core_api
{
	class Renderer2D
	{
	public:
		~Renderer2D();

		static void Initialize();
		static void Begin();
		static void End();

		static void DrawQuad(math::Vector<3, float> pos, math::Vector<4, float> color);

	private:


	private:
		static void Flush();
		static void FlushAndReset();

	};

}


