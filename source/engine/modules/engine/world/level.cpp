#include "level.hpp"
#include "actors/static_quad_actor.hpp"
#include "engine/state/state.hpp"
#include "resources/resource_manager.hpp"
#include <renderer_core/renderer.hpp>
#include <renderer_core/command_context.hpp>

#include "core/json/json.hpp"

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

bool Level::LoadLevel()
{
	IFile* path = gFileSystem.OpenFile("level_test.json", eRead);
	String data = {};
	data.resize(path->Size());
	path->Read((u8*)data.data(), path->Size());

	nlohmann::json file = data.AsRawStr();

	check(!file["name"].is_null());
	check(!file["actors"].is_array());

	for (auto actorJson : file["actors"])
	{
		String typeName(actorJson["type"]);
		Type* type = RayState()->ObjectDb->GetTypeByName(typeName.AsRawStr()); // TODO
		check(type != nullptr);

		Actor* actor = (Actor*)type->CreateInstance();
		check(actor != nullptr);
		actor->GetComponents().Clear();

		for (auto componentJson : actorJson["components"])
		{
			String componentTypeName(componentJson["type"]);
			Type* componentType = RayState()->ObjectDb->GetTypeByName(componentTypeName.AsRawStr()); // TODO
			check(componentType != nullptr);

			IComponent* component = (IComponent*)componentType->CreateInstance();
			actor->GetComponents().PushBack(component);

			auto props = componentJson["properties"];
			
			for (FieldInfo& field : componentType->Fields)
			{
				auto prop = props[field.Name.AsRawStr()];

#define SET_PROP(x) *(x*)(component + field.Offset) = prop.get<x>();
				
				switch(field.Type)
				{
				case eU8:		SET_PROP(u8); break;
				case eU16:		SET_PROP(u16); break;
				case eU32:		SET_PROP(u32); break;
				case eU64:		SET_PROP(u64); break;
				case eS8:		SET_PROP(s8); break;
				case eS16:		SET_PROP(s16); break;
				case eS32:		SET_PROP(s32); break;
				case eS64:		SET_PROP(s64); break;
				case eF32:		SET_PROP(f32); break;
				case eF64:		SET_PROP(f64); break;
				case eBoolean:	SET_PROP(bool); break;
				case eString:	__debugbreak(); break;
				case eVector1:	__debugbreak(); break;
				case eVector2:	__debugbreak(); break;
				case eVector3:	__debugbreak(); break;
				case eVector4:	__debugbreak(); break;
				default: 
					__debugbreak();
				}
			}

			// TODO: actor props

			SpawnActor(actor);
		}
	}
	
	path->Close();
	delete path;

	return true;
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

