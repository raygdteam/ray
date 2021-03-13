#pragma once
#include <core/core.hpp>
#include <renderer_core/resources/gpu_texture.hpp>

#include <engine\world\components\transform.hpp>

// ------------- RENDER DATA -------------
struct RenderData
{};

struct StaticQuadRenderData : RenderData
{
	TextureView Texture;
};

// ------------- SCENE PROXY ------------
struct PrimitiveSceneProxy
{
	Transform* Transform;
};

struct StaticQuadSceneProxy : PrimitiveSceneProxy
{
	StaticQuadRenderData* RenderData;
};
