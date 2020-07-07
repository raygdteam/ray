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
void RAY_INPUT_API update();

void RAY_INPUT_API add_listener(ray::input::listener*);
void RAY_INPUT_API add_remove(ray::input::listener*);
	
}