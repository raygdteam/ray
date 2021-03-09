#pragma once
#include <core/core.hpp>
#include <core/math/vector.hpp>
#include <core/math/matrix.hpp>
#include <resources/resource_manager.hpp>
#include "renderer_core.hpp"
#include <engine/ui2/ext/imgui.h>

#define TEXTURE_ATLAS_INDEX 0
#define ENGINE_RENDER_TARGET_INDEX 1

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

/*
	SEQUENCE OF CALLS

	SetVertices(...)
	SetIndices(...)

	Begin(...)

	Draw calls...

	End(...)
*/
class RAY_RENDERERCORE_API UiRenderer
{
private:
	static RootSignature _uiRootSignature;
	static GraphicsPipeline _uiPipelineState;

	static DescriptorHeap _descriptorHeap;

public:
	static void Initialize(u32 w, u32 h, void* data) noexcept;

	// after vertices and indices got set we must invoke Begin()
	static void Begin(const FMatrix4x4& vp, GraphicsContext& gfxContext) noexcept;
	
	// draw calls
	static void Draw(size_t indexCount, size_t vertexOffset, size_t indexOffset, u32 texId, GraphicsContext& gfxContext) noexcept;
	static void DrawSceneRenderTarget(const FVector3& pos, const FVector2& size, GraphicsContext& gfxContext) noexcept;
	
	// it must be invoked after all draw calls
	static void End(GraphicsContext& gfxContext) noexcept;

public:
	// it must be invoked first of all to provide vertices and indices
	static void SetVertices(ImDrawVert* vertices, size_t count) noexcept;
	static void SetIndices(u32* indices, size_t count) noexcept;

};

