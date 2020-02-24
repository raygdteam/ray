#pragma once
#include <engine/engine/engine.hpp>

namespace ray::launcher
{

class EngineLoop : IEngineLoop
{
	IEngine* _engine;
public:
	EngineLoop() = default;
	~EngineLoop();


	void PreInitialize(pcwstr commandLine, s32 argc) override;
	void Initialize() override;

	void Tick() override;
};

}
