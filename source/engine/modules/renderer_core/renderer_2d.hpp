#pragma once

#include <core/math/vector.hpp>

namespace ray::renderer_core_api
{
	class GraphicsPipeline;
	class RootSignature;

	class Renderer2D
	{
	public:
		~Renderer2D();

		static void Initialize();
		static void Begin();
		static void End();

		static void Shutdown();

		static void DrawQuad(const FVector<3>& pos, const FVector<4>& color);
		static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, const FVector<4>& color);

	private:
		static GraphicsPipeline _2DPipeline;
		static RootSignature _2DSignature;

	private:
		static void Flush();
		static void FlushAndReset();

	};

}


