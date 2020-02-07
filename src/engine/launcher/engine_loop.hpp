#pragma once
#include <engine/engine/engine.hpp>

namespace ray::launcher
{

class EngineLoop : IEngineLoop
{
public:
	EngineLoop() = default;
	~EngineLoop() = default;


	void PreInitialize(pcwstr commandLine, s32 argc) override;
	void Initialize() override;

	void Tick() override;
};

}
