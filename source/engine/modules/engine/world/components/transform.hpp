#pragma once
#include <core/core.hpp>
#include <engine/world/components/component_base.hpp>
#include <core/math/vector.hpp>

class Transform : public IComponent
{
	RAYOBJECT_BODY(Transform, IComponent);
public:
	FVector<2> Position;
	FVector<2> Scale;

	Transform()
		: Position(FVector<2> {0.0f, 0.0f}), Scale(FVector<2> {0.0f, 0.0f})
	{ }

	Transform(const FVector<2>& position, const FVector<2>& scale)
		: Position(position), Scale(scale)
	{ }

	void Init() override {}
	void Tick() override {}
	void OnDestroy() override {}

	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;

	void LoadFromJson(JsonValue& json) override;
};
