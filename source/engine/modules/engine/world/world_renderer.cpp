#include <engine/world/world.hpp>
#include <engine/world/level.hpp>

#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_2d.hpp>
#include <renderer_core/command_context.hpp>

using namespace ray::renderer_core_api;

void World::Render()
{
	Level* level = _levelData->Level;
	const ActorData* actorData = level->_atd.GetData();
	(void)actorData;

	GraphicsContext& ctx = GraphicsContext::Begin();
	_renderer->BeginScene(ctx);
	Renderer2D::Begin(*_primaryCameraActor);

	// for (const ActorData& data : actorData)
	// {
	//		data.SceneProxy.RenderData
	//		...
	// }

	Renderer2D::End(ctx);
	_renderer->EndScene(ctx);
	// ctx.Finish(true);
}

void World::RendererInitialize()
{
	_renderer = new IRenderer;
	_renderer->Initialize(nullptr);
	Renderer2D::Initialize({}, {}, {});
}
