#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

namespace ray
{
RAYOBJECT("engine://world/world")
class World : RayObject
{
	RAYOBJECT_BODY(World);
};

}
