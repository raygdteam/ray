#pragma once

#include <core/math/vector.hpp>

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

class RTexture;

namespace ray::renderer_core_api
{
	class GraphicsPipeline;
	class RootSignature;
	class GraphicsContext;
	class UserDescriptorHeap;

	class RAY_RENDERERCORE_API Renderer2D
	{
	public:
		~Renderer2D();

		static void Initialize(RTexture& texture);
		static void Begin();
		static void End(GraphicsContext& gfxContext);

		static void Shutdown();

		static void DrawQuad(const FVector<3>& pos, const FVector<4>& color, GraphicsContext& gfxContext);
		static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, const FVector<4>& color, GraphicsContext& gfxContext);

	private:
		static GraphicsPipeline _2DPipeline;
		static RootSignature _2DSignature;
		static UserDescriptorHeap _descriptorHeap;

	private:
		static void Flush(GraphicsContext& gfxContext);
		static void FlushAndReset(GraphicsContext& gfxContext);

	};

}


