#pragma once
#include <engine/engine/engine.hpp>

class EngineLoop : IEngineLoop
{
	IEngine* _engine;
public:
	EngineLoop() = default;
	~EngineLoop();


	void PreInitialize() override;
	void Initialize() override;

	void Tick() override;
};
