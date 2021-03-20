#pragma once
#include <core/core.hpp>
#include <renderer_core/resources/gpu_texture.hpp>

#include <engine\world\components\transform.hpp>

// ------------- RENDER DATA -------------
struct RenderData
{
	u64 MaterialId;
};

// ------------- SCENE PROXY ------------
struct PrimitiveSceneProxy
{
	Transform* Transform;
	u64 MaterialId;
};

struct StaticQuadSceneProxy : PrimitiveSceneProxy
{
	//StaticQuadRenderData* RenderData;
};
