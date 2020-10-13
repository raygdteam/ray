#include "level.hpp"
#include "actors/static_quad_actor.hpp"
#include "engine/state/state.hpp"
#include "resources/resource_manager.hpp"
#include <renderer_core/renderer.hpp>
#include <renderer_core/command_context.hpp>
#include <renderer_core/resources/texture_manager.hpp>

Level::Level()
{
	
}

using namespace ray::renderer_core_api;

void Level::SpawnActor(Actor* actor)
{
	actor->Awake(); // ??
	_actors.PushBack(actor);

	StaticQuadActor* sqActor = dynamic_cast<StaticQuadActor*>(actor);

	CommandContext& ctx = CommandContext::Begin();
	RTexture* texture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync(sqActor->Material.TextureName, eTexture));

	if (texture == nullptr) *(u64*)0xFFFFFFFFFFFFFFFF = 0xDED;

	TextureManager textureManager;
	textureManager.PrepareTextures(ctx, &texture, 1, true);

	// TODO: memory leak and error
	StaticQuadSceneProxy* proxy = new StaticQuadSceneProxy;
	proxy->RenderData = new StaticQuadRenderData;
	proxy->RenderData->TextureId = texture->GetId();
	proxy->Transform = sqActor->GetTransform();
	_atd.PushBack(ActorData { actor, actor->ATD, proxy});
	
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

