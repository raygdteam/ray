#include <app_framework/base/platform_window.hpp>

#include <engine/world/world.hpp>
#include <engine/world/level.hpp>
#include <engine/state/state.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/command_context.hpp>

using namespace ray::renderer_core_api;

void World::Render()
{
	Level* level = _levelData->Level;
	
	GraphicsContext& ctx = GraphicsContext::Begin();
	_renderer->BeginScene(ctx);

	Renderer2D::Begin(*_primaryCameraActor);

	static bool BLACK_PAPER_MOON = false;

	for (size_t i = 0; i < level->_atd.Size(); ++i)
	{
		const ActorData& actorData = level->_atd[i];
		
		StaticQuadSceneProxy* proxy = static_cast<StaticQuadSceneProxy*>(actorData.SceneProxy);
		FVector<3> position =
		{
			proxy->Transform->Position.x,
			proxy->Transform->Position.y,
			1.f
		};

		FVector<2> scale =
		{
			1.f,
			1.f
		}; 

		Renderer2D::DrawQuad(position, scale, proxy->RenderData->TextureId, ctx);
	}

	Renderer2D::End(ctx);

	BLACK_PAPER_MOON = !BLACK_PAPER_MOON;
	//

	// for (const ActorData& data : actorData)
	// {
	//		data.SceneProxy.RenderData
	//		...
	// }

	//Renderer2D::End(ctx);
	_renderer->EndScene(ctx);
}

void World::RendererInitialize(ray::core::IPlatformWindow* window)
{
	_primaryCameraActor = new CameraActor();
	/*_renderer = new IRenderer;
	_renderer->Initialize(window);*/
	Renderer2D::Initialize();
	//Renderer2D::Initialize({}, {}, {});
}
