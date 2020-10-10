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
	const ActorData* actorData = level->_atd.GetData();
	(void)actorData;

	GraphicsContext& ctx = GraphicsContext::Begin();
	_renderer->BeginScene(ctx);

	float step = static_cast<float>(1.f / 15.f);

	static FVector<2> lava[4] =
	{
		{ 0.f, step },
		{ 0.f, 0.f },
		{ step, 0.f },
		{ step, step }
	};

	static FVector<2> water[4] =
	{
		{ 0.f, step * 3 },
		{ 0.f, step * 2 },
		{ step, step * 2 },
		{ step, step * 3 }
	};

	// calculated by calculator

	Renderer2D::Begin(*_primaryCameraActor);

	static bool BLACK_PAPER_MOON = false;

	Renderer2D::DrawQuad({ BLACK_PAPER_MOON ? 50.1f : -50.1f, 60.1f, .1f }, { 1.f, 1.f }, lava, ctx);// red, closer to camera
	Renderer2D::DrawQuad({ -0.1f, -0.1f, .1f }, { 1.f, 1.f }, water, ctx);
	Renderer2D::DrawQuad({ -100.5f, .5f, 0.1f }, { 1.f, 1.f }, water, ctx); // green, futher from camera

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
	_renderer = new IRenderer;
	_renderer->Initialize(window);
	RTexture* texture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync("/engine/atlas2.png", ResourceType::eTexture));
	Renderer2D::Initialize((void*)texture->GetData().GetData(), texture->GetDimensions().x, texture->GetDimensions().y);
	//Renderer2D::Initialize({}, {}, {});
}
