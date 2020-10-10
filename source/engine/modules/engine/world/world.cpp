#include "world.hpp"

#include <core/debug/assert.hpp>
#include <core/lib/thread.hpp>

#include <engine/world/level.hpp>
#include <resources/resource_manager.hpp>
#include <engine/state/state.hpp>

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
	IThread::Name("World tick Primary thread");
	while (true)
	{
		ReadyToTick.Wait(); // Wait for IEngine command to begin ticking
		RenderingFinished.Wait(); // Wait for renderer thread to finish
		// Tick(1.f / 75.f);
		TickActors(eEarlyTick, _delta);
		TickActors(ePrePhysicsUpdate, _delta);
		PhysicsUpdate();
		TickActors(ePostPhysicsUpdate, _delta);
		TickActors(ePostPhysicsUpdate, _delta);
		WorldTickFinished.Signal();  // Signal the level is ready to render
	}
}

void World::RenderingThread()
{
	IThread::Name("Renderer primary thread");
	while (true)
	{
		WorldTickFinished.Wait();
		Render();
		RenderingFinished.Signal();
		TickFinished.Signal();
	}
}

void World::Initialize(ray::core::IPlatformWindow* window)
{
	// load level
	_levelData = new WorldLevelData;
	_levelData->Level = new Level();
	_levelData->Level->_owningWorld = this;
	_levelData->Level->LoadTestLevel();

	/* BUG: assuming no server build */
	RendererInitialize(window);
	
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

void World::SetPrimaryCamera(CameraActor* camera)
{
	// BUG: assuming no concurrency
	_primaryCameraActor = camera;
}

