#include "pch.h"
#include "../Core/core_init.h"
#include "engine.hpp"
#include "irenderer.hpp"
#include "config/models/renderer.hpp"

class engine_impl
{
public:
	void preinitialize(ray::core::application* app);
	void initialize();
	void run();
	void destroy();
	bool on_window_close();
	ray::Level get_active_level();
	void schedule_renderer_reload();

private:
	ray::core::application* _current_app;
	bool bRunning;
	ray::renderer::IRenderer* _renderer;
	std::shared_ptr<ray::Level> _active_level;
	bool bWindowSizeChanged = false;
};

class actyor : public ray::IActor
{
public:
	actyor()
	{
		
	}
};

void engine_impl::preinitialize(ray::core::application* app)
{
	RAY_ASSERT(app, TEXT("Application is null!"))
	ray::core::preinitialize();
	Platform::Init(0, 0, 0);
	ray::file_system::query_mount(app->get_resources_path(), "/resources/");

	// TODO: temp solution. fix later
	this->_active_level.reset(new ray::Level);

	_current_app = app;
}

void engine_impl::initialize()
{
	spdlog::info("+------------------------------------+");
	spdlog::info("|            RAY ENGINE              |");
	spdlog::info("|      copyright (c) 2019-2020       |");
	spdlog::info("|        Ilya, Seva, Nikita          |");
	spdlog::info("+------------------------------------+");

	_renderer = ray::renderer::IRenderer::create_renderer(ray::renderer::Vk);
	_renderer->Init();

	_current_app->on_startup();


}

void engine_impl::run()
{
	bWindowSizeChanged = false;

	// for delta_time calculation
	static auto startTime = std::chrono::high_resolution_clock::now();

	while (Platform::WindowIsOpen())
	{
		Platform::OnEvent();

		if (bWindowSizeChanged)
		{
			bWindowSizeChanged = false;
			_renderer->WindowSizeChanged(Platform::GetWidth(), Platform::GetHeight());
		}

		if(!Platform::CanTick())
			continue;

		// level update
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - startTime).count();

		//_active_level->Tick(deltaTime);

		_renderer->BeginFrame();
		_renderer->Draw();
		_renderer->EndFrame();
	}
}

void engine_impl::destroy()
{
	_renderer->Destroy();

	delete _renderer;
	delete _current_app;
}

bool engine_impl::on_window_close()
{
	bRunning = false;
	return true;
}

ray::Level engine_impl::get_active_level()
{
	// TODO: Это не должно быть так.
	return *_active_level.get();
}

void engine_impl::schedule_renderer_reload()
{
	bWindowSizeChanged = true;
}

engine_impl _engine;

void ray::engine::engine::start(core::application* app)
{
	clock_t startup = clock();

	preinitialize(app);
	clock_t preinitializeEnd = clock();

	spdlog::info("preinitialization took {}ms", difftime(preinitializeEnd, startup));

	initialize();
	clock_t initializeeEnd = clock();

	spdlog::info("initialization took {}ms", difftime(initializeeEnd, startup));

	run();
	destroy();
}

void ray::engine::engine::preinitialize(core::application * app)
{
	_engine.preinitialize(app);
}

void ray::engine::engine::initialize()
{
	_engine.initialize();
}

ray::Level ray::engine::engine::get_active_level()
{
	return _engine.get_active_level();
}

void ray::engine::engine::schedule_renderer_reload()
{
	_engine.schedule_renderer_reload();
}

void ray::engine::engine::run()
{
	_engine.run();
}

void ray::engine::engine::destroy()
{
	_engine.destroy();
}