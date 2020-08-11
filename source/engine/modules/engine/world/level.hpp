#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

namespace ray
{
/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
RAYOBJECT("engine://world/level")
class Level : public RayObject
{
	RAYOBJECT_BODY(Level);

	PROPERTY(Serializable)
	ray::Array<ray::Actor*> _actors;
};

}
