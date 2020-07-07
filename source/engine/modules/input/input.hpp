#pragma once
#include <core/core.hpp>
#include "listener.hpp"

#ifdef RAY_BUILD_INPUT
#define RAY_INPUT_API RAY_DLLEXPORTS
#else
#define RAY_INPUT_API RAY_DLLIMPORT
#endif

namespace ray::input
{

void RAY_INPUT_API Initialize();
void RAY_INPUT_API Update();

void RAY_INPUT_API AddListener(ray::input::Listener*);
void RAY_INPUT_API AddRemove(ray::input::Listener*);
	
}