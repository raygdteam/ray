#include "world.hpp"

#include <core/debug/assert.hpp>
#include <core/lib/thread.hpp>

#include <engine/world/level.hpp>
#include <engine/state/state.hpp>
#include <engine/bundle/bundle_meta.hpp>

#include <resources/resource_manager.hpp>
#include "actors/static_quad_actor.hpp"

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

void World::LoadLevel(pcstr name)
{
	_levelData = new WorldLevelData;
	_levelData->Level = new Level();
	_levelData->Level->_owningWorld = this;
	// _levelData->Level->LoadTestLevel();
	
	(void)name;

	decltype(StaticQuadActor())* actor1 = new StaticQuadActor();
	decltype(StaticQuadActor())* actor2 = new StaticQuadActor();
	decltype(StaticQuadActor())* actor3 = new StaticQuadActor();
	actor2->Material.TextureName = "/engine/hero.png";
	actor3->Material.TextureName = "/engine/tex_.png";

	actor1->GetTransform()->Position = FVector2 { 100.f, 100.f };
	actor2->GetTransform()->Scale = FVector2 { 5.f, 5.f };
	actor2->GetTransform()->Position = FVector2 { 100.f, 100.f }.Multiply(3.f);
	actor2->GetTransform()->Position = FVector2{ 100.f, 100.f }.Multiply(-3.f);
	
	//_levelData->Level->SpawnActor(actor1);
	_levelData->Level->SpawnActor(actor2);
	//_levelData->Level->SpawnActor(actor3);
}

void World::Initialize(IPlatformWindow* window)
{
	_renderer = new IRenderer;
	_renderer->Initialize(window);

	// load level
	LoadLevel("\0");

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

