#include "world.hpp"

#include <core/debug/assert.hpp>

#include <engine/world/level.hpp>
#include <engine/resources/resource_manager.hpp>
#include <engine/state/state.hpp>

struct WorldLevelData
{
	Level* Level;
	/* for level streaming
	 * FVector3 Position;
	 */
};

void World::TickActors(ActorTickStage stage, f64 delta) const
{
	Level* level = _levelData->Level;
	
	for (ActorTickData& tickData : level->_atd)
	{
		if (tickData.Tick.Stage & stage)
			tickData.Actor->Tick(delta);
	}
}

void World::PhysicsUpdate()
{
}

void World::Render()
{
}

void World::Tick(f64 delta)
{
	if (_shouldLoadLevel)
	{
		ResourceManager* resourceManager = RayState()->ResourceManager;

		ResourceRef ref = resourceManager->ResolveReference(_levelRef);
		check(ref.Valid);

		/* Let other subsystems keep up */
		return;
	}

	TickActors(eEarlyTick, delta);
	TickActors(ePrePhysicsUpdate, delta);
	PhysicsUpdate();
	TickActors(ePostPhysicsUpdate, delta);
	TickActors(eLateUpdate, delta);

	Render();
}

