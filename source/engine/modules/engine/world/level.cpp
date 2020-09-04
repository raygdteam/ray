#include "level.hpp"

struct ActorTickData
{
	ActorTick& Tick;
	Actor* Actor;
};

Level::Level()
{
	
}

void Level::RebuildATD()
{
	(void)false;
}

void Level::SpawnActor(Actor* actor)
{
	actor->Awake(); // ??
	_actors.PushBack(actor);
	RebuildATD();
	actor->BeginPlay(); // ??
}

void Level::Tick(f64 delta)
{
	for (ActorTickData& tickData : _atd)
	{
		if (tickData.Tick.Stage & eEarlyTick)
			tickData.Actor->Tick(delta);
	}

	/* ... */

	for (ActorTickData& tickData : _atd)
	{
		if (tickData.Tick.Stage & eLateUpdate)
			tickData.Actor->Tick(delta);
	}
}

void Level::Serialize(Archive&)
{
	RebuildATD();
}

void Level::Deserialize(Archive&)
{
	
}

RAYOBJECT_DESCRIPTION_BEGIN(Level)
	RAYOBJECT_DESCRIPTION_CREATEABLE();
	RAYOBJECT_DESCRIPTION_NAME("engine://world/Level")
RAYOBJECT_DESCRIPTION_END(Level)

