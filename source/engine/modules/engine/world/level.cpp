#include "level.hpp"


Level::Level()
{
	
}

void Level::SpawnActor(Actor* actor)
{
	_actors.PushBack(actor);
}

void Level::Serialize(Archive&)
{
	
}

void Level::Deserialize(Archive&)
{
	
}

RAYOBJECT_DESCRIPTION_BEGIN(Level)
	RAYOBJECT_DESCRIPTION_NAME("engine://world/Level")
RAYOBJECT_DESCRIPTION_END(Level)

