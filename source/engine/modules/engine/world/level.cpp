#include "level.hpp"
#include "actors/static_quad_actor.hpp"
#include "engine/state/state.hpp"
#include "resources/resource_manager.hpp"
#include <renderer_core/renderer.hpp>
#include <renderer_core/command_context.hpp>

Level::Level()
{
	
}

void Level::SpawnActor(Actor* actor)
{
	actor->Awake(); // ??
	_actors.PushBack(actor);

	// TODO: memory leak and error
	PrimitiveSceneProxy* proxy = nullptr;

	StaticQuadActor* sqActor = dynamic_cast<StaticQuadActor*>(actor);

	if (sqActor != nullptr)
	{
		StaticQuadSceneProxy* sqProxy = new StaticQuadSceneProxy;

		//CommandContext& ctx = CommandContext::Begin();
		//(void)ctx;
		RTexture* texture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync(sqActor->Material.TextureName, eTexture));

		if (texture == nullptr) *(u64*)0xFFFFFFFFFFFFFFFF = 0xDED;

		// TODO

		//TextureManager textureManager;
		//textureManager.PrepareTextures(ctx, &texture, 1, true);
		sqProxy->RenderData = new StaticQuadRenderData;
		sqProxy->RenderData->TextureId = texture->GetId();
		sqProxy->Transform = actor->GetTransform();

		proxy = sqProxy;
	}
	else
	{
		proxy = new PrimitiveSceneProxy;
		proxy->Transform = actor->GetTransform();
	}

	if (_jobCurrentSize >= ChunkSize)
	{
		_jobCurrentNum += 1;
		_jobCurrentSize = 0;
		_jobs.PushBack(new ActorTickJob());
	}
	_jobs.At(_jobCurrentNum)->Actors.PushBack(actor);
	
	_atd.PushBack(ActorData { actor, actor->ATD, proxy });
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

Array<Actor*>& Level::GetActors()
{
	return _actors;
}

void Level::Serialize(Archive&)
{
}

void Level::Deserialize(Archive&)
{
	
}

RAYOBJECT_DESCRIPTION_BEGIN(Level)
	RAYOBJECT_DESCRIPTION_CREATEABLE();
	RAYOBJECT_DESCRIPTION_NAME("engine://world/Level")
RAYOBJECT_DESCRIPTION_END(Level)

