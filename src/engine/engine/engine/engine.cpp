#include "pch.hpp"
#include "core/memory/memory.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

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