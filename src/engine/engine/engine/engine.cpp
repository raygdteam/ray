#include "pch.hpp"
#include "core/memory/memory.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

#include "core/file_system/file_system.hpp"

using namespace ray::core;

// плохо!
#include <ray/os/include.hpp>
#undef CreateWindow

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

	typedef IRenderer* (*GetRendererApi_t)();
	HMODULE handle = LoadLibrary("renderer_gl");
	GetRendererApi_t getRendererApi = (GetRendererApi_t)GetProcAddress(handle, "GetRendererApi");

	_renderer = getRendererApi();
	_renderer->Initialize(window);
	
	window->SetWindowVisibility(true);

	_window = window;
}

void RayEngine::Tick()
{
	static_cast<core::IPlatformWindow*>(_window)->Update();
	if (static_cast<core::IPlatformWindow*>(_window)->ShouldClose())
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


void _DllMainCRTStartup() {}
MEMORY_OVERRIDE