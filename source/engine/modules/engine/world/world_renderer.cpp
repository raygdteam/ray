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
	
	gRootObject->RenderAll();
	_renderer->EndScene(ctx);
}

void World::RendererInitialize(IPlatformWindow* window)
{
	_primaryCameraActor = new CameraActor();
	/*_renderer = new IRenderer;
	_renderer->Initialize(window);*/
	Renderer2D::Initialize();
	//Renderer2D::Initialize({}, {}, {});
}
