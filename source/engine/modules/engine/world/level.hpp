#pragma once
#include <core/object/object.hpp>
#include <core/lib/array.hpp>
#include <core/threading/thread_pool.hpp>
#include <engine/world/actor.hpp>
#include <engine/world/world_render_data.hpp>

struct ActorData
{
	Actor* Actor;
	ActorTick Tick;
	PrimitiveSceneProxy* SceneProxy;
	ThreadPoolJob* TickJob;
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

	class ActorTickJob : public ThreadPoolJob
	{
		Array<Actor*> _actors;
		f64 _dt;

	public:
		ActorTickJob(Array<Actor*>& actor, f64 dt)
		{
			_actors = actor;
			_dt = dt;
		}

		void SetDelta(f64 dt)
		{
			_dt = dt;
		}

		void Run() override
		{
			for (Actor* actor : _actors)
			{
				actor->Tick(_dt);
			}
		}
	};
};
