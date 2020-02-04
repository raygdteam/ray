#pragma once
#include <ray/type/extensions.hpp>
#include "core/raytl/array.hpp"
#include "engine/engine_def.hpp"

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
class RAY_ENGINE_API RayEngine : IEngine
{
	IEngineLoop* _engineLoop;
	raytl::Array<pcstr> _rendererInterface;
public:
	RayEngine();

	void Initialize(IEngineLoop* engineLoop) override;
	void Tick() override;
};

// TODO: Phew! Global variable!
extern RAY_ENGINE_API IEngine* gEngine;

/** Request engine exit - be it normal exit or crash. */
bool IsEngineExitRequested() noexcept;
void RequestEngineExit(bool data);

}