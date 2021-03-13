#include <app_framework/base/platform_window.hpp>

#include <engine/world/world.hpp>
#include <engine/world/level.hpp>
#include <engine/state/state.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/command_context.hpp>
#include <renderer_core/resources/buffer_manager.hpp>

// #include <engine/ui/widget.hpp>
#include <engine/ui2/ui2.hpp>

// extern UiWidget* gWidget;
UiRootObject* gRootObject;

double currentFrame = 0.f;

void World::Render()
{
	GraphicsContext& ctx = GraphicsContext::Begin();

	_renderer->Begin(gSceneColorBuffer, ctx);

	Renderer2D::Begin(_primaryCameraActor->GetCameraComponent()->GetViewProjection());
	for (size_t i = 0; i < 100; ++i)
	{
		for (size_t j = 0; j < 100; ++j)
		{
			FVector<3> pos = { -1500.f + i * 40, 900.f - j * 40, static_cast<f32>(1.0f) };
			FVector<2> size = { 0.15f, 0.15f };
			f32 r = 1.f; //  Sin(Pi() / 2 + (i * _delta))
			f32 g = 0.5f; //  Cos(Pi() / 2 + (j * _delta))
			FVector<4> color = { r, g, r * g, 1.f };
			Renderer2D::DrawQuad(pos, size, color, ctx);
		}
	}
	Renderer2D::End(ctx);

	_renderer->End(gSceneColorBuffer, ctx);

	/*bool g_bEditor = false;
	if (g_bEditor)
	{
		_renderer->Begin(gEditorColorBuffer, ctx);
		// UiRenderer
		gRootObject->RenderAll(ctx);

		_renderer->End(gEditorColorBuffer, ctx);

		//_renderer->Present(gEditorColorBuffer, ctx);
	}
	else
	{
		//_renderer->Present(gSceneColorBuffer, ctx);
	}*/
	
	//Renderer2D::Begin(_primaryCameraActor->GetCameraComponent()->GetViewProjection());

	//Level* level = _levelData->Level;

	////Renderer2D::Begin(*_primaryCameraActor);

	////static bool BLACK_PAPER_MOON = false;

	//for (size_t i = 0; i < level->_atd.Size(); ++i)
	//{
	//	const ActorData& actorData = level->_atd[i];
	//	
	//	StaticQuadSceneProxy* proxy = static_cast<StaticQuadSceneProxy*>(actorData.SceneProxy);

	//	if (proxy == nullptr) continue;
	//	
	//	FVector<3> position =
	//	{
	//		proxy->Transform->Position.x,
	//		proxy->Transform->Position.y,
	//		1.f
	//	};
	//	
	//	FVector2 size = { 0.15f, 0.15f };
	//	FVector4 color = { 1.f, 1.f, 0.f, 1.f };
	//	Renderer2D::DrawQuad(position, size, color, ctx);
	//	
	//	/*currentFrame += (0.007f * _delta);
	//	if (currentFrame >= 3)
	//		currentFrame = 0;

	//	FVector<2> textureCoords[4] =
	//	{
	//		{ static_cast<u32>(currentFrame) * .3333f, .25f },
	//		{ static_cast<u32>(currentFrame) * .3333f, 0.f },
	//		{ (1 + static_cast<u32>(currentFrame)) * .3333f, 0.f },
	//		{ (1 + static_cast<u32>(currentFrame)) * .3333f, .25f }
	//	};*/

	//	//Renderer2D::DrawQuad(position, proxy->Transform->Scale, proxy->RenderData->TextureId, textureCoords, ctx);
	//}

	//Renderer2D::End(ctx);
	//

	//for (size_t i = 0; i < 500; ++i)
	//{
	//	for (size_t j = 0; j < 200; ++j)
	//	{
	//		FVector<3> pos = { -1500.f + i * 40, 900.f - j * 40, static_cast<f32>(1.0f) };
	//		FVector<2> size = { 0.15f, 0.15f };
	//		auto r = Sin(Pi() / 2 + (i * _delta)); //   i * j + 100 * 28
	//		auto g = Cos(Pi() / 2 + (j * _delta)); // i * j + 100 * 28
	//		FVector<4> color = { r, g, r * g, 1.f };
	//		Renderer2D::DrawQuad(pos, size, color, ctx);
	//	}
	//}

	//Renderer2D::End(ctx);
}

void World::RenderEditor(GraphicsContext& ctx)
{
	_renderer->Begin(gSceneColorBuffer, ctx);

	Renderer2D::Begin(_primaryCameraActor->GetCameraComponent()->GetViewProjection());
	Level* level = _levelData->Level;
	
	for (size_t i = 0; i < level->_atd.Size(); ++i)
	{
		const ActorData& actorData = level->_atd[i];
		
		StaticQuadSceneProxy* proxy = static_cast<StaticQuadSceneProxy*>(actorData.SceneProxy);

		if (proxy == nullptr) continue;
		
		FVector<3> position =
		{
			proxy->Transform->Position.x,
			proxy->Transform->Position.y,
			1.f
		};
		
		FVector2 size = { 0.75f, 0.75f };
		FVector4 color = { 1.f, 1.f, 0.f, 1.f };
		
		Renderer2D::DrawQuad(position, size, color, ctx);

	}
	Renderer2D::End(ctx);

	_renderer->End(gSceneColorBuffer, ctx);
}

#include <engine/world/actors/static_quad_actor.hpp>

void World::RendererInitialize(IPlatformWindow* window)
{
	_levelData->Level->SpawnActor(_primaryCameraActor);

	//StaticQuadActor* actor1 = new StaticQuadActor();
	//actor1->GetTransform()->Position = FVector2{ 100, 100 };
	//actor1->Material.TextureName = "/engine/hero.png";

	//StaticQuadActor* actor2 = new StaticQuadActor();
	//actor2->GetTransform()->Position = FVector2{ 500, 500 };
	//actor2->Material.TextureName = "/engine/atlas2.png";

	//_levelData->Level->SpawnActor(actor1);
	//_levelData->Level->SpawnActor(actor2);

	/*_renderer = new IRenderer;
	_renderer->Initialize(window);*/
	RTexture* whiteTexture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync("/engine/white_texture.jpg", eTexture));
	Renderer2D::Initialize(*whiteTexture);
	//Renderer2D::Initialize({}, {}, {});
}
