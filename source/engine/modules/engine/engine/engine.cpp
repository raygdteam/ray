#include "pch.hpp"
#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

#include <input/input.hpp>

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

	_renderer = new IRenderer;
	_renderer->Initialize(window);
	
	_logger = new Logger("engine");

	input::add_listener(this);

	window->SetWindowVisibility(true);

	_window = window;
}

void RayEngine::Tick()
{
	input::update();

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

void RayEngine::on_key_down(int key)
{
	_logger->Log("key is down");
}

void RayEngine::on_key_up(int key)
{
	_logger->Log("key is up");
}

void RayEngine::on_mouse_move(int x, int y)
{

}

void RayEngine::on_left_mouse_down(int x, int y)
{
	_logger->Log("left mouse is down");
}

void RayEngine::on_left_mouse_up(int x, int y)
{
	_logger->Log("left mouse is up");
}

void RayEngine::on_right_mouse_down(int x, int y)
{
	_logger->Log("right mouse is down");
}

void RayEngine::on_right_mouse_up(int x, int y)
{
	_logger->Log("right mouse is up");
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
