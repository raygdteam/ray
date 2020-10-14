#pragma once
#include <core/core.hpp>

struct UiRenderData
{
	u64 Type;
	bool Optimize : 8;

	u64 State;

	struct {
		u64 RenderType;
		
		f32* Vertices;
		f32* TextureCoordinates;
		u64* TextureIdxs;
	}* StateDecsriptions;
};
