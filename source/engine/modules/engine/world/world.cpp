#include "world.hpp"

#include <core/debug/assert.hpp>
#include <core/lib/thread.hpp>

#include <engine/world/level.hpp>
#include <engine/state/state.hpp>
#include <engine/bundle/bundle_meta.hpp>

#include <resources/resource_manager.hpp>
#include "actors/static_quad_actor.hpp"

#include <renderer_core/resources/upload_buffer.hpp>
#include "core/threading/thread_pool.hpp"
#include "engine/ui2/ui2.hpp"

//UploadBuffer* gUploadBuffer;

void World::TickActors(f64 delta) const
{
	Level* level = _levelData->Level;
	//_pool.Wait();

	static bool bOnce = false;
	if (!bOnce)
	{
		for (Level::ActorTickJob* job : level->_jobs)
			_pool.SubmitWork(job);

		_pool.Wait();
		bOnce = !bOnce;
	}
	else
	{
		_pool.ResubmitWork();
		_pool.Wait();
	}
	

	/*for (ActorData& tickData : level->_atd)
	{
		tickData.Actor->Tick(delta);
	}*/
}

void World::PhysicsUpdate()
{
}

extern UiRootObject* gRootObject;

void World::WorldTickThread()
{
	IThread::Name("World tick Primary thread");
	while (true)
	{
		ReadyToTick.Wait(); // Wait for IEngine command to begin ticking
		RenderingFinished.Wait(); // Wait for renderer thread to finish
		/* swap buffers */
		
		//Tick(1.f / 75.f);
		//
		gRootObject->Tick();
		
		TickActors(_delta);
		//PhysicsUpdate();
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

	//decltype(StaticQuadActor())* actor1 = new StaticQuadActor();
	//decltype(StaticQuadActor())* actor2 = new StaticQuadActor();
	//decltype(StaticQuadActor())* actor3 = new StaticQuadActor();
	//actor2->Material.TextureName = "/engine/hero.png";
	//actor3->Material.TextureName = "/engine/tex_.png";

	//actor1->GetTransform()->Position = FVector2 { 100.f, 100.f };
	//actor2->GetTransform()->Scale = FVector2 { 5.f, 5.f };
	//actor2->GetTransform()->Position = FVector2 { 100.f, 100.f }.Multiply(3.f);
	//actor2->GetTransform()->Position = FVector2{ 100.f, 100.f }.Multiply(-3.f);
	
	//_levelData->Level->SpawnActor(actor1);
	//_levelData->Level->SpawnActor(actor2);
	//_levelData->Level->SpawnActor(actor3);

	for (u32 i = 0; i < 500; ++i)
	{
		for (u32 j = 0; j < 500; ++j)
		{
			StaticQuadActor* actor = new StaticQuadActor();
			Transform* transform = actor->GetTransform();

			transform->Position = FVector2 { -1500.f + i * 40, 900.f - j * 40 };
			_levelData->Level->SpawnActor(actor);
		}
	}
}

World::World() : _pool(gThreadPoolManager->Allocate())
{
	
}

World::~World()
{
	
}

void World::Destroy() noexcept
{
	_renderer->Shutdown();
	delete _renderer;
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

	//_pool = gThreadPoolManager->Allocate();

	gRootObject->Initialize(window);

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
