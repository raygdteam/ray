#pragma once
#include <core/core.hpp>
#include <engine/world/components/component_base.hpp>
#include <core/math/vector.hpp>

struct Transform : IComponent
{
	FVector<2> Position;
	// FVector<2> Scale;

	void Init() override {}
	void Tick() override {}
	void OnDestroy() override {}
};
