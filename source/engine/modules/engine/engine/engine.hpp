#pragma once

//#define _TEMP_NO_RENDERER_CORE_API_

#include "engine/engine/engine_def.hpp"

#ifndef _TEMP_NO_RENDERER_CORE_API_
#include <renderer_core/renderer.hpp>
#endif

// #include <renderer_core/renderer_commands.hpp>

#ifndef _TEMP_NO_RENDERER_CORE_API_
using namespace ray::renderer_core_api;
#endif

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
class RAY_ENGINE_API RayEngine : public IEngine
{
	IEngineLoop* _engineLoop;
	void* _window = nullptr;
#ifndef _TEMP_NO_RENDERER_CORE_API_
	IRenderer* _renderer = nullptr;
#endif
public:
	RayEngine();
	~RayEngine();

	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;
};

/** Request engine exit - be it normal exit or crash. */
RAY_ENGINE_API bool IsEngineExitRequested() noexcept;
RAY_ENGINE_API void RequestEngineExit(bool data);

}
