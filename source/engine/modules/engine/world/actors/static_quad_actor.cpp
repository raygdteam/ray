#include "static_quad_actor.hpp"

StaticQuadActor::StaticQuadActor()
{
	Material.TextureName = "/engine/tex.png";
}

void StaticQuadActor::Awake()
{
	
}

void StaticQuadActor::BeginPlay()
{
}

void StaticQuadActor::Tick(f64 delta)
{
}

void StaticQuadActor::OnDestroy()
{
}

RAYOBJECT_DESCRIPTION_BEGIN(StaticQuadActor)
RAYOBJECT_DESCRIPTION_NAME("engine://world/actors/StaticQuadActor")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(StaticQuadActor);
