#pragma once
#include <core/core.hpp>
#include <core/math/vector.hpp>
#include <core/lib/array.hpp>

struct UiRenderData
{
	/*u64 Type;
	u64 Flags;
	u64 State;

	Array<UiRenderDataState> StateDescriptions;*/
	FVector3 Position;
	FVector2 Scale;
	Array<FVector2> TexCoord;

	u32 TextureIndex;
};
