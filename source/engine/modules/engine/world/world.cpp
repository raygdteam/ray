#include "world.hpp"

#include <core/debug/assert.hpp>
#include <core/lib/thread.hpp>

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
	
	for (ActorData& tickData : level->_atd)
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
	Level* level = _levelData->Level;
	const ActorData* actorData = level->_atd.GetData();

	// for (const ActorData& data : actorData)
	// {
	//		data.RenderData
	//		...
	// }
}

void World::WorldTickThread()
{
	ReadyToTick.Wait(); // Wait for IEngine command to begin ticking
	RenderingFinished.Wait(); // Wait for renderer thread to finish
	// Tick();
	WorldTickFinished.Signal();  // Signal the level is ready to render
}

void World::RenderingThread()
{
	WorldTickFinished.Wait();
	Render();
	RenderingFinished.Signal();
}

void World::Initialize()
{
	// load level
	_levelData->Level = new Level();
	_levelData->Level->LoadTestLevel();
	
	IThread::Start([this] { this->WorldTickThread(); });
	IThread::Start([this] { this->RenderingThread(); });
}

void World::Tick(f64 delta)
{
	RenderingFinished.Wait();
	if (_shouldLoadLevel)
	{
		ResourceManager* resourceManager = RayState()->ResourceManager;

		ResourceRef ref = resourceManager->ResolveReference(_levelRef);
		check(ref.Valid);

		/* Let other subsystems keep up */
		return;
	}

	ReadyToTick.Signal();
}

