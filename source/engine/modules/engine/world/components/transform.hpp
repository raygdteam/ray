#pragma once
#include <core/core.hpp>
#include <engine/world/components/component_base.hpp>
#include <core/math/vector.hpp>

struct Transform : IComponent
{
	FVector<2> Position;
	// FVector<2> Scale;

	Transform()
		: Position(FVector<2> {0.0f, 0.0f})
	{ }

	Transform(const FVector<2>& position)
		: Position(position)
	{ }

	void Init() override {}
	void Tick() override {}
	void OnDestroy() override {}
};
