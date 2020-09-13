#include "level.hpp"
#include "actors/static_quad_actor.hpp"


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

void Level::LoadTestLevel()
{
	StaticQuadActor* actor1 = new StaticQuadActor();
	actor1->GetTransform()->Position = FVector2 { 100, 100 };
	
	StaticQuadActor* actor2 = new StaticQuadActor();
	actor2->GetTransform()->Position = FVector2 { 500, 500 };

	SpawnActor(actor1);
	SpawnActor(actor2);
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

