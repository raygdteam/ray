#pragma once
#include <ray/type/extensions.hpp>

namespace ray::launcher
{

class EngineLoop
{
public:
	EngineLoop() = default;
	~EngineLoop() = default;


	void PreInitialize(pcwstr commandLine, s32 argc);
	void Initialize();

	void Tick();
};

}
