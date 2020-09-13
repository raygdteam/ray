#pragma once
#include <core/core.hpp>

// ------------- RENDER DATA -------------
struct RenderData
{};

struct StaticQuadRenderData : RenderData
{};

// ------------- SCENE PROXY ------------
struct PrimitiveSceneProxy
{
	RenderData* RenderData;
};

struct StaticQuadSceneProxy : PrimitiveSceneProxy
{};
