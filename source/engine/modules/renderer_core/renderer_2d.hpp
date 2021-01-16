#pragma once
#include <core/math/vector.hpp>
#include <resources/resource_manager.hpp>
#include <engine/world/actors/camera_actor.hpp>
#include "resources/ring_buffer.hpp"

#ifdef RAY_BUILD_RENDERER_CORE
#define RAY_RENDERERCORE_API __declspec(dllexport)
#else
#define RAY_RENDERERCORE_API RAY_DLLIMPORT
#endif

class GraphicsPipeline;
class RootSignature;
class GraphicsContext;
class DescriptorHeap;

class RAY_RENDERERCORE_API Renderer2D
{
private:
	static GraphicsPipeline _2DPipeline;
	static RootSignature _2DSignature;
	static DescriptorHeap _descriptorHeap;
	static RingBuffer _ringBuffer;

public:
	~Renderer2D();

public:
	static void Initialize(/*TextureManager* textureManager*/);
	static void Shutdown();

public:
	static void Begin(CameraActor& camera);
	static void Begin(const FMatrix4x4& viewProjection);
	static void End(GraphicsContext& gfxContext);

public:
	static void SetCamera(CameraActor& camera);

public:
	//static void DrawQuad(const FVector<3>& pos, FVector<2>* textureCoords, GraphicsContext& gfxContext);
	static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, u32 textureIndex, FVector<2>* textureCoords, GraphicsContext& gfxContext);
	static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, u32 textureIndex, GraphicsContext& gfxContext);
	static void DrawQuad(const FVector<3>& pos, const FVector<2>& size, const FVector<4>& color, GraphicsContext& gfxContext);

private:
	static void Begin();
	static void Flush(GraphicsContext& gfxContext);
	static void FlushAndReset(GraphicsContext& gfxContext);

};
