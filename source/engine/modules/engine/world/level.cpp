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

