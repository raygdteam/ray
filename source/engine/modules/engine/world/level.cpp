#include "level.hpp"


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

