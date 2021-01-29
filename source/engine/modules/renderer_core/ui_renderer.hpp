#pragma once
#include <core/core.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>
#include <resources/resource_manager.hpp>
#include "renderer_core.hpp"
#include <engine/ui2/ext/imgui.h>

class GraphicsContext;
class ColorBuffer;
class RootSignature;
class GraphicsPipeline;
class DescriptorHeap;

struct RAY_RENDERERCORE_API UiVertex
{
	FVector2 Position;
	FVector2 TexCoords;
	u32 Color;
};



class RAY_RENDERERCORE_API UiRenderer
{
private:
	static RootSignature _uiRootSignature;
	static GraphicsPipeline _uiPipelineState;

	static DescriptorHeap _descriptorHeap;

public:
	static void Initialize(u32 w, u32 h, void* data) noexcept;

	static void Begin(const FMatrix4x4& vp) noexcept;
	static void Draw(ImDrawVert* vertices, size_t verticesCount, u32* indices, size_t indicesCount, GraphicsContext& gfxContext) noexcept;
	static void DrawSceneRenderTarget(const FVector3& pos, const FVector2& size, GraphicsContext& gfxContext) noexcept;
	static void End(GraphicsContext& gfxContext) noexcept;

private:
	static void Flush(GraphicsContext& gfxContext) noexcept;
	static void FlushAndReset(GraphicsContext& gfxContext) noexcept;

};

