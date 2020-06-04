#pragma once
#include <ray/type/extensions.hpp>
#include "core/raytl/array.hpp"
#include "engine/engine/engine_def.hpp"
#include <vector>
#include "renderer_core/renderer.hpp"

using namespace ray::renderer_core_api;

namespace ray
{

/**
 * Represents the engine loop object, where all the magic is being called.
 */
struct IEngineLoop
{
	virtual void PreInitialize(pcwstr commandLine, s32 argc) = 0;
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
private:
	IEngineLoop* _engineLoop;
	std::vector<pcstr> _rendererInterface;
	void* _window;
	IRenderer* _renderer;

public:
	RayEngine();
	~RayEngine();

	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;

};

// TODO: Phew! Global variable!
extern RAY_ENGINE_API IEngine* gEngine;

/** Request engine exit - be it normal exit or crash. */
RAY_ENGINE_API bool IsEngineExitRequested() noexcept;
RAY_ENGINE_API void RequestEngineExit(bool data);

}
