#include "pch.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

#include "state/state.hpp"


using namespace ray::core;

namespace ray
{

RayEngine::RayEngine() : _engineLoop(nullptr)
{

}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	ray::core::IPlatformWindow* window = core::IPlatformWindow::CreateInstance();
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");

	// Load renderer module
	auto res = RayState()->ModuleManager->LoadModule("renderer_dx12");
	if (!res.IsSuccess()) __debugbreak();

	_renderer = new IRenderer;
	_renderer->Initialize(window, res.Data);
	
	window->SetWindowVisibility(true);

	_window = window;
}

void RayEngine::Tick()
{
	static_cast<core::IPlatformWindow*>(_window)->Update();
	
	//for debugging
	bool bShouldClose = static_cast<core::IPlatformWindow*>(_window)->ShouldClose();
	if (bShouldClose)
	{
		ray::RequestEngineExit(true);
		return;
	}
	_renderer->Draw();
}

RayEngine::~RayEngine()
{
	static_cast<core::IPlatformWindow*>(_window)->Destroy();
	static_cast<core::IPlatformWindow*>(_window)->Shutdown();
	_renderer->Shutdown();

	delete _renderer;
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
