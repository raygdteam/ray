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

void World::WorldTickThread()
{
	while (true)
	{
		ReadyToTick.Wait(); // Wait for IEngine command to begin ticking
		RenderingFinished.Wait(); // Wait for renderer thread to finish
		// Tick();
		WorldTickFinished.Signal();  // Signal the level is ready to render
	}
}

void World::RenderingThread()
{
	while (true)
	{
		WorldTickFinished.Wait();
		Render();
		RenderingFinished.Signal();
		TickFinished.Signal();
	}
}

void World::Initialize()
{
	// load level
	_levelData = new WorldLevelData;
	_levelData->Level = new Level();
	_levelData->Level->LoadTestLevel();
	
	IThread::Start([this] { this->WorldTickThread(); })->Start();
	IThread::Start([this] { this->RenderingThread(); })->Start();

	RenderingFinished.Signal();
	TickFinished.Signal();
}

void World::Tick(f64 delta)
{
	TickFinished.WaitFor();
	_delta = delta;
	//if (_shouldLoadLevel)
	//{
	//	ResourceManager* resourceManager = RayState()->ResourceManager;

	//	ResourceRef ref = resourceManager->ResolveReference(_levelRef);
	//	check(ref.Valid);

	//	/* Let other subsystems keep up */
	//	return;
	//}
	//
	static bool once = true;
	if (once)
	{
		RenderingFinished.Signal();
		once = false;
	}

	ReadyToTick.Signal();
}

