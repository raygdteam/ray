#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

namespace ray
{
RAYOBJECT("engine://world/level")
class Level : public RayObject
{
	RAYOBJECT_BODY(Level);

	PROPERTY(Serializable)
	ray::Array<ray::Actor*> _actors;
};

}
