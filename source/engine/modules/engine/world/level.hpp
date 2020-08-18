#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
RAYOBJECT("engine://world/level")
class Level : public RayObject
{
	RAYOBJECT_BODY(Level, RayObject);

	// Temp
public:
	PROPERTY(Serializable)
	Array<Actor*> _actors;

	Level();

	void SpawnActor(Actor* actor);

	// TEMPORARY
	void LoadLevel();
	void Tick(f64 delta);

	friend class Actor;
};
