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

		static void DrawQuad(FVector<3> pos, FVector<4> color);

	private:


	private:
		static void Flush();
		static void FlushAndReset();

	};

}


