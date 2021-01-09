#include "engine.hpp"
#include <app_framework/base/platform_window.hpp>

#include <core/lib/thread.hpp>
#include <core/log/log.hpp>
#include <core/module/module.hpp>

#include <input/input.hpp>

#include <engine/state/state.hpp>

#include <resources/resource.hpp>
#include <resources/resource_manager.hpp>

#include <renderer_core/command_context.hpp>
#include <renderer_core/renderer_2d.hpp>

// ALL OF THIS IS TEMP
#include <chrono>
#include "core/debug/assert.hpp"
#include <engine/world/world.hpp>


#undef CreateWindow

static World* gWorld;
u64 tempLastTime = 0;

using namespace ray;
using namespace ray::core;

static Logger* eng;

RayEngine::RayEngine() : _engineLoop(nullptr)
{
	eng = new Logger("engine");
}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	IRayState* state = RayState();

	eng->Log("Initializing Ray engine");
	eng->Log("version %s.%s.%s [%s]", RAY_VERSION_MAJOR, RAY_VERSION_MINOR, RAY_VERSION_PATCH, RAY_VERSION_CODENAME);
	eng->Log("built on \"%s\"", __TIMESTAMP__);

	IPlatformWindow* window = IPlatformWindow::CreateInstance();
	
	window->Initialize();
	window->CreateWindow("RAY_ENGINE");

	state->Input->Initialize(window);

	eng->Log("renderer load begin");

	// Load renderer module
	//auto res = RayState()->ModuleManager->LoadModule("renderer_core");
	//if (!res.IsSuccess()) __debugbreak();
	//
	//_renderer = new IRenderer;
	//_renderer->Initialize(window);

	gWorld = new World();
	gWorld->Initialize(window);

	//gWidget->Update(); // Widget and all it's children has to tick at least once in order to build RenderData
	
	//
	//RTexture* texture = dynamic_cast<RTexture*>(RayState()->ResourceManager->LoadResourceSync("/engine/atlas2.png", ResourceType::eTexture));
	//Renderer2D::Initialize((void*)texture->GetData().GetData(), texture->GetDimensions().x, texture->GetDimensions().y);

	eng->Log("renderer load end");
	
	window->SetWindowVisibility(true);
	_window = window;
}

void RayEngine::Tick()
{
	static f64 delta = 0;
	auto __start = std::chrono::high_resolution_clock::now();

	RayState()->Input->Reset();
	
	static_cast<IPlatformWindow*>(_window)->Update();
	bool bShouldClose = static_cast<IPlatformWindow*>(_window)->ShouldClose();
	if (bShouldClose)
	{
		RequestEngineExit(true);
		return;
	}

	gWorld->Tick(delta);

	auto elapsed = std::chrono::high_resolution_clock::now() - __start;
	delta = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000.f;
}

RayEngine::~RayEngine()
{
	static_cast<IPlatformWindow*>(_window)->Destroy();
	static_cast<IPlatformWindow*>(_window)->Shutdown();
	//_renderer->Shutdown();

	//delete _renderer;
	delete (IPlatformWindow*)_window;
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
