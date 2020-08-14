#include "level.hpp"


Level::Level()
{
	
}

void Level::SpawnActor(Actor* actor)
{
	_actors.PushBack(actor);
}

