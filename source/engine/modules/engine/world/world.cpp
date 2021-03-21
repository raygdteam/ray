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
#include "renderer_core/renderer_2d.hpp"

//UploadBuffer* gUploadBuffer;

void World::TickActors(f64 delta) const
{
	//Level* level = _levelData->Level;
	//_pool.Wait();

	/*static bool bOnce = false;
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
	}*/
	

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

void World::LoadLevelInternal(String& path)
{
	_levelData = new WorldLevelData;
	_levelData->Level = new Level();
	_levelData->Level->_owningWorld = this;
	_levelData->Level->LoadFrom(path);
	_primaryCameraActor = new CameraActor();

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

	/*for (u32 i = 0; i < 500; ++i)
	{
		for (u32 j = 0; j < 500; ++j)
		{
			StaticQuadActor* actor = new StaticQuadActor();
			Transform* transform = actor->GetTransform();

			transform->Position = FVector2 { -1500.f + i * 40, 900.f - j * 40 };
			transform->Scale = FVector2{ 0.5f, 0.5f };
			_levelData->Level->SpawnActor(actor);
		}
	}*/
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
	if (window != nullptr)
	{
		_renderer = new IRenderer;
		_renderer->Initialize(window);
	}
	
	// load level
	String name("../../engine/resources/level.json");
	LoadLevelInternal(name);
	
	if (window == nullptr) return;

	/* BUG: assuming no server build */
	RendererInitialize(window);
	
	IThread::Start([this] { this->WorldTickThread(); })->Start();
	IThread::Start([this] { this->RenderingThread(); })->Start();

	//_pool = gThreadPoolManager->Allocate();
	gRootObject = new UiRootObject;
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

u64 World::CompileMaterial(MaterialCompileProperties& props)
{
	RTexture* texture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync(props.Texture.AsRawStr(), eTexture));

	if (texture == nullptr) *(u64*)0xFFFFFFFFFFFFFFFF = 0xDED;

	auto textureDesc = GpuTextureDescription::Texture2D(texture->GetDimensions().x, texture->GetDimensions().y, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D12_RESOURCE_FLAG_NONE);
	gUploadBuffer->SetTextureData(textureDesc, texture->GetData().GetData());
	GpuTexture* actorTexture = new GpuTexture();
	actorTexture->Create(textureDesc, "material texture");

	TextureView actorTextureView;
	actorTextureView.Create(*actorTexture);

	MaterialInstance entry = {
		.Id = _materialInstances.Size(),
		.Name = props.Name,
		.Texture = actorTexture,
		.TextureView = actorTextureView
	};
	
	_materialInstances.PushBack(entry);
	return entry.Id;
}

MaterialInstance& World::GetMaterialInstance(u64 id)
{
	return _materialInstances[id];
}

u64 World::GetMaterialIdForName(String& name)
{
	for (MaterialInstance& materialInstance : _materialInstances)
	{
		if (materialInstance.Name == name) return materialInstance.Id;
	}
	return -1;
}

void World::LoadLevel(String& path)
{
	for (MaterialInstance& materialInstance : _materialInstances)
	{
		materialInstance.Texture->Release();
		materialInstance.Texture->Destroy();
		delete materialInstance.Texture;
	}

	_materialInstances.Clear();
	
	Level* level = _levelData->Level;

	for (Actor* actor : level->GetActors())
	{
		actor->OnDestroy();
		delete actor;
	}

	delete level;
	
	LoadLevelInternal(path);
}
