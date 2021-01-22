#include <app_framework/base/platform_window.hpp>

#include <engine/world/world.hpp>
#include <engine/world/level.hpp>
#include <engine/state/state.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/command_context.hpp>

// #include <engine/ui/widget.hpp>
#include <engine/ui2/ui2.hpp>

// extern UiWidget* gWidget;
UiRootObject* gRootObject;

double currentFrame = 0.f;

void World::Render()
{
	//Level* level = _levelData->Level;
	
	GraphicsContext& ctx = GraphicsContext::Begin();
	_renderer->BeginScene(ctx);

	//Renderer2D::Begin(*_primaryCameraActor);

	//static bool BLACK_PAPER_MOON = false;

	//for (size_t i = 0; i < level->_atd.Size(); ++i)
	//{
	//	const ActorData& actorData = level->_atd[i];
	//	
	//	StaticQuadSceneProxy* proxy = static_cast<StaticQuadSceneProxy*>(actorData.SceneProxy);
	//	FVector<3> position =
	//	{
	//		proxy->Transform->Position.x,
	//		proxy->Transform->Position.y,
	//		1.f
	//	};

	//	currentFrame += (0.007f * _delta);
	//	if (currentFrame >= 3)
	//		currentFrame = 0;

	//	FVector<2> textureCoords[4] =
	//	{
	//		{ static_cast<u32>(currentFrame) * .3333f, .25f },
	//		{ static_cast<u32>(currentFrame) * .3333f, 0.f },
	//		{ (1 + static_cast<u32>(currentFrame)) * .3333f, 0.f },
	//		{ (1 + static_cast<u32>(currentFrame)) * .3333f, .25f }
	//	};

	//	Renderer2D::DrawQuad(position, proxy->Transform->Scale, proxy->RenderData->TextureId, textureCoords, ctx);
	//}

	//Renderer2D::End(ctx);
	//

	Renderer2D::Begin(*_primaryCameraActor);

	for (size_t i = 0; i < 500; ++i)
	{
		for (size_t j = 0; j < 200; ++j)
		{
			FVector<3> pos = { -1500.f + i * 10, 900.f - j * 10, static_cast<f32>(1.0f) };
			FVector<2> size = { 0.05f, 0.05f };
			auto r = Sin(i * j + 100 * 28);
			auto g = Cos(i * j + 100 * 28);
			FVector<4> color = { r, g, r * g, 1.f };
			Renderer2D::DrawQuad(pos, size, color, ctx);
		}
	}

	//FVector3 pos = { 0.f, 0.f, 1.f };
	//FVector4 color = { 1.f, 0.f, 0.f, 1.f };
	//FVector2 size = { 1.f, 1.f };
	//Renderer2D::DrawQuad(pos, size, color, ctx);

	//Renderer2D::End(ctx);

	//Renderer2D::Begin(*_primaryCameraActor);

	//pos.y = 500.f;
	//color.x = 0.f;
	//color.y = 1.f;

	//Renderer2D::DrawQuad(pos, size, color, ctx);
	//
	Renderer2D::End(ctx);

	_renderer->EndScene(ctx);
}

void World::RendererInitialize(IPlatformWindow* window)
{
	_primaryCameraActor = new CameraActor();
	_levelData->Level->SpawnActor(_primaryCameraActor);
	/*_renderer = new IRenderer;
	_renderer->Initialize(window);*/
	Renderer2D::Initialize();
	//Renderer2D::Initialize({}, {}, {});
}
