#pragma once
#include <core/object/object.hpp>

RAYOBJECT("engine://world/world")
class World : RayObject
{
	RAYOBJECT_BODY(World);

	PROPERTY(Serializable)
	// ray::Array<IActor*> 
};

struct IRayObjectSystem_Meta_World
{
	IRayObjectSystem_Meta_World()
	{
		__debugbreak();
	}
};

extern IRayObjectSystem_Meta_World Metadsgsdgsdag;
