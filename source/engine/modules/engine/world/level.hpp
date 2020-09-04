#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>

// Internal use only.
struct ActorTickData;

/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
class Level final : public RayObject
{
	RAYOBJECT_BODY(Level, RayObject);
	Array<ActorTickData> _atd;

	void RebuildATD();
	// Temp
public:
	Array<Actor*> _actors;

	Level();

	void SpawnActor(Actor* actor);

	// TEMPORARY
	void LoadLevel();
	void Tick(f64 delta);


	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
	
	friend class Actor;
};
