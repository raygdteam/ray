#pragma once
#include <core/core.hpp>

#include <engine\world\components\transform.hpp>

// ------------- RENDER DATA -------------
struct RenderData
{};

struct StaticQuadRenderData : RenderData
{
	u64 TextureId;
};

// ------------- SCENE PROXY ------------
struct PrimitiveSceneProxy
{};

struct StaticQuadSceneProxy : PrimitiveSceneProxy
{
	StaticQuadRenderData* RenderData;
	Transform* Transform;
};
