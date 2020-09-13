#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>
#include <engine/world/world_render_data.hpp>

struct ActorData
{
	Actor* Actor;
	ActorTick& Tick;
	PrimitiveSceneProxy* RenderData;
};

/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
class Level final : public RayObject
{
	RAYOBJECT_BODY(Level, RayObject);
	
	friend class World;
	friend class Actor;

	Array<Actor*> _actors;
	Array<ActorData> _atd;

	void RebuildATD();
public:
	Level();

	void SpawnActor(Actor* actor);

	// TEMPORARY
	void LoadTestLevel();

	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
};
