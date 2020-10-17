#pragma once
#include <core/core.hpp>
#include <core/math/vector.hpp>
#include <core/lib/array.hpp>

struct QuadVertex
{
	FVector<3> Position;
	FVector<2> TexCoord;

	u32 TextureIndex;
};

enum RenderType
{
	eNull,
	eRawData,
	eCustom = eRawData,

	eButton
};

struct UiRenderDataState
{
	u64 RenderType;

	u64 NumVertices;
	QuadVertex* Vertices;
};

struct UiRenderData
{
	u64 Type;
	u64 Flags;
	u64 State;

	Array<UiRenderDataState> StateDescriptions;
};
