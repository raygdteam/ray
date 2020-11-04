#pragma once
#include <core/core.hpp>
#include <core/math/vector.hpp>
#include <core/lib/array.hpp>
#include <engine/world/components/transform.hpp>

struct UiRenderData
{
	/*u64 Type;
	u64 Flags;
	u64 State;

	Array<UiRenderDataState> StateDescriptions;*/
	Transform* Transform;
	Array<FVector2> TexCoord;

	u32 TextureIndex;
};
