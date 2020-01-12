#include "pch.h"
#include "../Core/core_init.h"
#include "engine.hpp"
#include "irenderer.hpp"
#include "event.hpp"
#include "window_event.h"
#include "Level.hpp"

class engine_impl
{
public:
	void preinitialize(ray::core::application* app);
	void initialize();
	void run();
	void destroy();
	void on_event(Event& e);
	bool on_window_close(Event& e);
	ray::Level get_active_level();
	void schedule_renderer_reload();

private:
	ray::core::application* _current_app;
	bool bRunning;
	ray::renderer::IRenderer* _renderer;
	std::shared_ptr<ray::Level> _active_level;
	bool bWindowSizeChanged = false;
};

void engine_impl::preinitialize(ray::core::application* app)
{
	RAY_ASSERT(app, TEXT("Application is nullpointer!"))
	ray::core::preinitialize();
	ray::file_system::query_mount(app->get_resources_path(), "/resources/");

	_current_app = app;
}

void engine_impl::initialize()
{
	spdlog::info("+------------------------------------+");
	spdlog::info("|            RAY ENGINE              |");
	spdlog::info("|      copyright (c) 2019-2020       |");
	spdlog::info("|        Ilya, Seva, Nikita          |");
	spdlog::info("+------------------------------------+");

	Platform::SetCallback(std::bind(&engine_impl::on_event, this, std::placeholders::_1));
	_renderer = ray::renderer::IRenderer::create_renderer(ray::renderer::eRendererType::Vk);
	_renderer->Init();

	_current_app->on_startup();
}

void engine_impl::run()
{
	bWindowSizeChanged = false;
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

		_renderer->BeginFrame();

		//drawing
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

void engine_impl::on_event(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(std::bind(&engine_impl::on_window_close, this, std::placeholders::_1));
}

bool engine_impl::on_window_close(Event & e)
{
	bRunning = false;
	return true;
}

ray::Level engine_impl::get_active_level()
{
	// TODO: это не должно быть так!
	return *_active_level.get();
}

void engine_impl::schedule_renderer_reload()
{
	bWindowSizeChanged = true;
}

engine_impl _engine;

void ray::engine::engine::start(core::application* app)
{
	time_t startup;
	time(&startup);
	
	preinitialize(app);
	initialize();

	double initTime = difftime(time(0), startup);
	spdlog::info("initialization took {}s", initTime);

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
