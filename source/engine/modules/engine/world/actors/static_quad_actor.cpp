#include "static_quad_actor.hpp"
#include <engine/world/components/rendering_properties.hpp>

StaticQuadActor::StaticQuadActor()
{
	ATD.Stage = ePrePhysicsUpdate;
	ATD.Active = true;
}

StaticQuadActor::~StaticQuadActor()
{
	
}

void StaticQuadActor::Awake()
{
	
}

void StaticQuadActor::BeginPlay()
{
}

static u64 gAAAAAA = 0x4;

void StaticQuadActor::Tick(f64 delta)
{
	for (u64 i = 0; i < 100; ++i)
	{
		(void)i;
		gAAAAAA &= 0x88;
	}
}

void StaticQuadActor::OnDestroy()
{
}

RAYOBJECT_DESCRIPTION_BEGIN(StaticQuadActor)
RAYOBJECT_DESCRIPTION_NAME("engine://world/actors/StaticQuadActor")
RAYOBJECT_DESCRIPTION_CREATEABLE()
RAYOBJECT_DESCRIPTION_END(StaticQuadActor);
