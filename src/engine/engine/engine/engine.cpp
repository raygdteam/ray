#include "core/memory/memory.hpp"
#include "engine.hpp"

namespace ray
{

RayEngine::RayEngine() : _engineLoop(nullptr)
{

}

void RayEngine::Initialize(IEngineLoop* engineLoop)
{
	_engineLoop = engineLoop;

	// Ensures that RayEngine object won't get destroyed by accident.
	// TODO: ЧТО. ЭТО. ЗА. ХРЕНЬ???????
	{ static volatile RayEngine _ = *this; }

}

void RayEngine::Tick()
{

}

/************************************/
bool gEngineExitRequested = false;

bool IsEngineExitRequested() noexcept
{
	return gEngineExitRequested;
}

void RequestEngineExit(bool data)
{
	gEngineExitRequested = data;
}
}


void _DllMainCRTStartup() {}
MEMORY_OVERRIDE