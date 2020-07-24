#include "pch.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>
#include <core/lib/thread.hpp>
#include "state/state.hpp"
#include <core/log/log.hpp>

using namespace ray::core;

Logger* eng;

namespace ray
{

RayEngine::RayEngine() : _engineLoop(nullptr)
{
	eng = new Logger("engine");
}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	ray::file_system::FileSystem example;

	example.MountDirectory("example1/", "example/");

	example.FileExists("example1/what.txt");

	eng->Log("Initializing Ray engine...");
	eng->Log("version %s.%s.%s [%s]", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);
	eng->Log("built on \"%s\"", __TIMESTAMP__);

	eng->Log("[1/4] Window init...");
	ray::core::IPlatformWindow* window = core::IPlatformWindow::CreateInstance();
	
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");

	eng->Log("[2/4] Renderer load...");

	// Load renderer module
	auto res = RayState()->ModuleManager->LoadModule("renderer_dx12");
	if (!res.IsSuccess()) __debugbreak();

	eng->Log("[3/4] Renderer init...");
	
	_renderer = new IRenderer;
	_renderer->Initialize(window, res.Data);

	eng->Log("[4/4] Finishing...");
	
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
	_renderer->BeginScene();

	// renderer commands ...

	_renderer->Execute();
	_renderer->EndScene();
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
