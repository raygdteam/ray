#pragma once
#include <core/math/vector.hpp>
#include <resources/resource_manager.hpp>
#include <engine/world/actors/camera_actor.hpp>
#include "renderer_core.hpp"

class GraphicsPipeline;
class RootSignature;
class GraphicsContext;
class DescriptorHeap;
class DescriptorHandle;
class TextureView;

class RAY_RENDERERCORE_API Renderer2D
{
private:
	static GraphicsPipeline _2DPipeline;
	static RootSignature _2DSignature;
	static DescriptorHeap _descriptorHeap;

public:
	~Renderer2D();

public:
	static void Initialize(RTexture& whiteTexture);
	static void Shutdown();

public:
	static void Begin(const FMatrix4x4& viewProjection);
	static void End(GraphicsContext& gfxContext);

public:
	static void DrawQuad(const FVector3& pos, const FVector2& size, const TextureView& textureHandle, FVector2* textureCoords, GraphicsContext& gfxContext);
	static void DrawQuad(const FVector3& pos, const FVector2& size, const TextureView& textureHandle, GraphicsContext& gfxContext);
	
	static void DrawQuad(const FVector3& pos, const FVector2& size, const FVector4& color, GraphicsContext& gfxContext);

private:
	static void Begin();
	static void Flush(GraphicsContext& gfxContext);
	static void FlushAndReset(GraphicsContext& gfxContext);

};
