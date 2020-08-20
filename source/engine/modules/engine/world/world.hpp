#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

namespace ray
{
class World final : public RayObject
{
	RAYOBJECT_BODY(World, RayObject);

public:
	void Serialize(Archive&) override {}
	void Deserialize(Archive&) override {}
};

}
