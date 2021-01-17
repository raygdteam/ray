#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <engine/world/actor.hpp>
#include <engine/world/world_render_data.hpp>

struct ActorData
{
	Actor* Actor;
	ActorTick Tick;
	PrimitiveSceneProxy* SceneProxy;
};

/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
class RAY_ENGINE_API Level final : public RayObject
{
	RAYOBJECT_BODY(Level, RayObject);
	
	friend class World;
	friend class Actor;

	Array<Actor*> _actors;
	Array<ActorData> _atd;

	World* _owningWorld = nullptr;
public:
	Level();

	void SpawnActor(Actor* actor);

	// TEMPORARY
	void LoadTestLevel();

#ifdef RAY_EDITOR
	Array<Actor*>& GetActors();
#endif
	
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;
};
