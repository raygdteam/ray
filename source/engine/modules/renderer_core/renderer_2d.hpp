#pragma once
#include <core/math/vector.hpp>
#include <resources/resource_manager.hpp>
#include <engine/world/actors/camera_actor.hpp>
#include <renderer_core/resources/texture_manager.hpp>

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

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

		static void Initialize(/*TextureManager* textureManager*/);
		static void Begin(CameraActor& camera);
		static void Begin(const FMatrix4x4& viewProjection);
		static void End(GraphicsContext& gfxContext);

		static void SetCamera(CameraActor& camera);
		
		static void Shutdown();

		//static void DrawQuad(const FVector<3>& pos, FVector<2>* textureCoords, GraphicsContext& gfxContext);
		static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, u32 textureIndex, FVector<2>* textureCoords, GraphicsContext& gfxContext);
		static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, u32 textureIndex, GraphicsContext& gfxContext);

	private:
		static GraphicsPipeline _2DPipeline;
		static RootSignature _2DSignature;
		static UserDescriptorHeap _descriptorHeap;
		/*static TextureManager* _textureManager;*/

	private:
		static void Begin();
		static void Flush(GraphicsContext& gfxContext);
		static void FlushAndReset(GraphicsContext& gfxContext);

	};

}


