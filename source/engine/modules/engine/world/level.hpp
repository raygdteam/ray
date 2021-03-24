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
};

const u32 ChunkSize = 2048;

/**
 * Represents a collection of Actors and all necessary data for level to render.
 */
class RAY_ENGINE_API Level final : public RayObject
{
	RAYOBJECT_BODY(Level, RayObject);
	
	friend class World;
	friend class Actor;
	
public:
	class ActorTickJob;
private:
	
	Array<Actor*> _actors;
	Array<ActorData> _atd;
	Array<ActorTickJob*> _jobs;
	u32 _jobCurrentNum = -1;
	u32 _jobCurrentSize = ChunkSize;

	World* _owningWorld = nullptr;
public:
	Level();

	void SpawnActor(Actor* actor);

	// TEMPORARY
	void LoadTestLevel();
	bool LoadLevel();
	void LoadFrom(String& path);

#ifdef RAY_EDITOR
	Array<Actor*>& GetActors();
	void RebuildATD();
	void EditorCallbackOnActorModified(Actor* actor);
#endif
	
	void Serialize(Archive&) override;
	void Deserialize(Archive&) override;

	class ActorTickJob : public ThreadPoolJob
	{
	public:
		Array<Actor*> Actors;
		f64 Delta;

		ActorTickJob()
		{
			
		}

		void Run() override
		{
			for (Actor* actor : Actors)
			{
				actor->Tick(Delta);
			}
		}
	};
};
