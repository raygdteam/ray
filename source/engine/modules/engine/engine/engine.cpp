#include "pch.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>
#include <core/lib/thread.hpp>
#include "state/state.hpp"
#include <core/log/log.hpp>
#include <core/module/module.hpp>

// ALL OF THIS IS TEMP
#include <renderer_null/renderer.hpp>
#include <engine/world/level.hpp>
#include <chrono>
#include "core/debug/assert.hpp"

Renderer* tempRenderer = nullptr;
Level* tempLevel = nullptr;
u64 tempLastTime = 0;

using namespace ray::core;

namespace ray
{

static Logger* eng;

RayEngine::RayEngine() : _engineLoop(nullptr)
{
	eng = new Logger("engine");
	tempRenderer = new Renderer;
	tempLevel = new Level;
}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	assert(1 > 5, eng);
	assert(1 < 5, eng);

	_engineLoop = engineLoop;

	eng->Log("Initializing Ray engine");
	eng->Log("version %s.%s.%s [%s]", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);
	eng->Log("built on \"%s\"", __TIMESTAMP__);

	eng->Log("[1/4] Window init...");
	
#ifndef _TEMP_NO_RENDERER_CORE_API_
	ray::core::IPlatformWindow* window = core::IPlatformWindow::CreateInstance();
	
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");
#endif
	eng->Log("[2/4] Renderer load...");

#ifndef _TEMP_NO_RENDERER_CORE_API_
	// Load renderer module
	auto res = RayState()->ModuleManager->LoadModule("renderer_core");
	if (!res.IsSuccess()) __debugbreak();
#endif

	eng->Log("[3/4] Renderer init...");
	tempRenderer->Initialize();
	tempLevel->LoadLevel();

#ifndef _TEMP_NO_RENDERER_CORE_API_
	_renderer = new IRenderer;
	_renderer->Initialize(window, res.Data);
#endif

	eng->Log("[4/4] Finishing...");
	
	// window->SetWindowVisibility(true);
#ifndef _TEMP_NO_RENDERER_CORE_API_
	_window = window;
#endif
}

void RayEngine::Tick()
{
	static f64 delta = 0;
	auto __start = std::chrono::high_resolution_clock::now();
	
#ifndef _TEMP_NO_RENDERER_CORE_API_
	static_cast<core::IPlatformWindow*>(_window)->Update();
#endif
	
	//for debugging
	bool bShouldClose = tempRenderer->ShouldClose(); //static_cast<core::IPlatformWindow*>(_window)->ShouldClose();
	if (bShouldClose)
	{
		ray::RequestEngineExit(true);
		return;
	}

#ifndef _TEMP_NO_RENDERER_CORE_API_
	_renderer->BeginScene();

	// renderer commands ...

	_renderer->Execute();
	_renderer->EndScene();
#else
	tempLevel->Tick(delta);
	tempRenderer->BeginFrame();
	for (u32 i = 0; i < tempLevel->_actors.Size(); ++i)
	{
		Actor* actor = tempLevel->_actors[i];
		Transform* transform = actor->GetTransform();
		
		tempRenderer->DrawRect(transform->Position.x, transform->Position.y, 100, 100, {1.0f, 1.0f, 1.0f});
	}
	tempRenderer->EndFrame();
#endif
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
}


// void _DllMainCRTStartup() {}
