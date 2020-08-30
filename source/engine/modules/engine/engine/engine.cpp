#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>
#include <core/lib/thread.hpp>
#include "engine/state/state.hpp"
#include <core/log/log.hpp>
#include <core/module/module.hpp>

// ALL OF THIS IS TEMP
#include <engine/world/level.hpp>
#include <chrono>
#include "core/debug/assert.hpp"

#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>

#undef CreateWindow

Level* tempLevel = nullptr;
u64 tempLastTime = 0;

using namespace ray;
using namespace ray::core;

static log::logger* eng;

RayEngine::RayEngine() : _engineLoop(nullptr)
{
	eng = new log::logger("engine");
	tempLevel = new Level;
}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	eng->Log("Initializing Ray engine");
	eng->Log("version {}.{}.{} [{}]", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);
	eng->Log("built on \"{}\"", __TIMESTAMP__);

	eng->Log("[1/4] Window init...");
	ray::core::IPlatformWindow* window = core::IPlatformWindow::CreateInstance();
	
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");

	eng->Log("[2/4] Renderer load...");

	// Load renderer module
	auto res = RayState()->ModuleManager->LoadModule("renderer_core");
	if (!res.IsSuccess()) __debugbreak();
	
	_renderer = new IRenderer;
	_renderer->Initialize(window);
	Renderer2D::Initialize();

	eng->Log("[4/4] Finishing...");
	
	window->SetWindowVisibility(true);
	_window = window;
}

struct PingPongFloat
{
	float Data = 0.0f;
	float Step = 0.0001f;
	float From = -1.0f;
	float To = 1.0f;
	bool Forward = true;
	

	float DoStep()
	{
		if (Forward)
		{
			if (Data >= To)
			{
				Forward = !Forward;
				return Data;
			}
			Data += Step;
			return Data;
		}
		
		if (Data <= From)
		{
			Forward = !Forward;
			return Data;
		}
		Data -= Step;
		return Data;
	}
};

void RayEngine::Tick()
{
	static f64 delta = 0;
	auto __start = std::chrono::high_resolution_clock::now();
	
	static_cast<core::IPlatformWindow*>(_window)->Update();
	//for debugging
	bool bShouldClose = static_cast<core::IPlatformWindow*>(_window)->ShouldClose();
	if (bShouldClose)
	{
		RequestEngineExit(true);
		return;
	}

	GraphicsContext& gfxContext = GraphicsContext::Begin();

	_renderer->BeginScene(gfxContext);

#pragma clang diagnostic ignored "-Wreorder-init-list"

	//static PingPongFloat flt1;
	//static PingPongFloat flt2;
	//static bool depth = true;

	//static PingPongFloat clr1 {.From = 0.0f, .Step = 0.0025f };
	//static PingPongFloat clr2 { .From = 0.0f, .Step = 0.0025f };
	//static PingPongFloat clr3 { .From = 0.0f, .Step = 0.0025f };

	Renderer2D::Begin();

	//Renderer2D::DrawQuad({ -flt1.DoStep(), flt2.DoStep(), depth ? 0.1f : 0.2f }, { clr1.DoStep(), clr2.DoStep(), clr2.DoStep(), 0.f }, gfxContext);
	//Renderer2D::DrawQuad({ flt2.DoStep(), -flt2.DoStep(), depth ? 0.2f : 0.1f }, { clr2.DoStep(), clr3.DoStep(), clr1.DoStep(), 0.f }, gfxContext);

	//Renderer2D::DrawQuad({ flt1.DoStep(), flt2.DoStep(), depth ? 0.1f : 0.2f }, { clr1.DoStep(), clr2.DoStep(), clr2.DoStep(), 0.f }, gfxContext);
	//Renderer2D::DrawQuad({ -flt2.DoStep(), flt2.DoStep(), depth ? 0.2f : 0.1f }, { clr2.DoStep(), clr3.DoStep(), clr1.DoStep(), 0.f }, gfxContext);

	//Renderer2D::DrawQuad({ flt1.DoStep(), -flt2.DoStep(), depth ? 0.1f : 0.2f }, { clr1.DoStep(), clr2.DoStep(), clr2.DoStep(), 0.f }, gfxContext);
	//Renderer2D::DrawQuad({ -flt2.DoStep(), flt2.DoStep(), depth ? 0.2f : 0.1f }, { clr2.DoStep(), clr3.DoStep(), clr1.DoStep(), 0.f }, gfxContext);

	Renderer2D::DrawQuad({ .7f, .5f, 0.1f }, { 0.5f, 1.f }, { 1.f, 0.f, 0.f, 0.f }, gfxContext);// red, closer to camera
	Renderer2D::DrawQuad({ .5f, .5f, 0.5f }, { 1.f, 0.5f }, { 0.f, 1.f, 0.f, 0.f }, gfxContext); // green, futher from camera

	Renderer2D::End(gfxContext);

	// renderer commands ...
	_renderer->EndScene(gfxContext);

	// depth = !depth;

	/*tempLevel->Tick(delta);
	tempRenderer->BeginFrame();
	for (u32 i = 0; i < tempLevel->_actors.Size(); ++i)
	{
		Actor* actor = tempLevel->_actors[i];
		Transform* transform = actor->GetTransform();
		
		tempRenderer->DrawRect(transform->Position.x, transform->Position.y, 100, 100, {1.0f, 1.0f, 1.0f});
	}
	tempRenderer->EndFrame();*/
	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}

RayEngine::~RayEngine()
{
#ifndef _TEMP_NO_RENDERER_CORE_API_
	static_cast<core::IPlatformWindow*>(_window)->Destroy();
	static_cast<core::IPlatformWindow*>(_window)->Shutdown();
	_renderer->Shutdown();

	delete _renderer;
	delete (core::IPlatformWindow*)_window;
#endif
}

/************************************/
bool gEngineExitRequested = false;

bool IsEngineExitRequested() noexcept
{
	return gEngineExitRequested;
}

void RequestEngineExit(bool data)
{
	gEngineExitRequested = data;
}

// void _DllMainCRTStartup() {}
