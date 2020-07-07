#pragma once
#include "engine/engine/engine_def.hpp"
#include <vector>
#include <renderer_core/renderer.hpp>
#include <renderer_core/renderer_commands.hpp>

#include <input/listener.hpp>
#include <core/log/log.hpp>

using namespace ray::renderer_core_api;

namespace ray
{

/**
 * Represents the engine loop object, where all the magic is being called.
 */
struct IEngineLoop
{
	virtual void PreInitialize() = 0;
	virtual void Initialize() = 0;

	virtual void Tick() = 0;

	virtual ~IEngineLoop() = default;
};


// Base class for game engine and editor.
struct IEngine
{
	virtual void Initialize(IEngineLoop* engineLoop) = 0;
	virtual void Tick() = 0;

	virtual ~IEngine() = default;
};

/**
 *  The main Ray Engine class.
 */
class RAY_ENGINE_API RayEngine : public IEngine, public input::listener
{
	IEngineLoop* _engineLoop;
	std::vector<pcstr> _rendererInterface;
	void* _window;
	IRenderer* _renderer;
	Logger* _logger;

public:
	RayEngine();
	~RayEngine();

	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;


	// Унаследовано через listener
	virtual void on_key_down(int) override;

	virtual void on_key_up(int) override;

	virtual void on_mouse_move(int, int) override;

	virtual void on_left_mouse_down(int, int) override;

	virtual void on_left_mouse_up(int, int) override;

	virtual void on_right_mouse_down(int, int) override;

	virtual void on_right_mouse_up(int, int) override;

};

// TODO: Phew! Global variable!
extern RAY_ENGINE_API IEngine* gEngine;

/** Request engine exit - be it normal exit or crash. */
RAY_ENGINE_API bool IsEngineExitRequested() noexcept;
RAY_ENGINE_API void RequestEngineExit(bool data);

}
